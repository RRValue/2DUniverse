#include "HalfEdge2D/HalfEdge/HESCheck.h"

#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

HESCheck::HESCheck()
{
    reset();
}

HESCheck::HESCheck(HESMesh* const mesh)
{
    reset();

    m_SourceMesh = mesh;
    
    run();
    clear();
}

HESCheck::~HESCheck()
{

}

void HESCheck::reset()
{
    if(m_Error == E_HESCE_OK)
        for(auto& m : m_Meshes)
            delete m;
    
    m_Meshes.clear();

    m_HasPartConnectedInOneVertex = false;
    m_HasParts = false;
    m_HasHoles = false;
    m_Error = E_HESCE_NOMESH;
    
    m_SourceMesh = nullptr;
    m_ProcessingMesh = nullptr;
}

const HESCheck::HESCheckError& HESCheck::getError() const
{
    return m_Error;
}

HESCheck::HESMeshVector HESCheck::getMeshes()
{
    HESMeshVector res = m_Meshes;

    m_Error = E_HESCE_NOMESH;
    reset();

    return res;
}

void HESCheck::run(HESMesh* const mesh)
{
    reset();

    m_SourceMesh = mesh;

    run();
    clear();
}

void HESCheck::run()
{
    m_Error = E_HESCE_OK;

    if(m_SourceMesh == nullptr)
        m_Error = E_HESCE_NOMESH;
    else
        m_Error = E_HESCE_OK;

    if(m_Error != E_HESCE_OK)
        return;

    m_ProcessingMesh = new HESMesh(*m_SourceMesh);

    // check for connected parts in one vertex
    hasPartsConnectedInOneVertex();

    if(!m_PartsConnectingVertices.empty())
    {
        m_HasPartConnectedInOneVertex = true;
        m_HasParts = true;
    }

    if(m_HasPartConnectedInOneVertex)
        splitPartsConnectedInOneVertex();

    // find boundaries
    findBoundaries();

    if(m_Error != E_HESCE_OK)
        return;

    if(m_Boundaries.size() > 1)
        m_HasParts = true;

    findParts();

    for(const auto& p : m_MeshParts)
        if(p.m_NumHoles > 0)
        {
            m_HasHoles = true;

            break;
        }

    if(m_MeshParts.size() == 1)
        m_HasParts = false;

    if(m_Error != E_HESCE_OK)
        return;

    m_Meshes.clear();

    createMeshesFromParts();
}

void HESCheck::clear()
{
    m_PartsConnectingVertices.clear();
    m_Boundaries.clear();
    m_MeshParts.clear();

    if(m_ProcessingMesh != nullptr)
        delete m_ProcessingMesh;
    
    m_ProcessingMesh = nullptr;
}

void HESCheck::hasPartsConnectedInOneVertex()
{
    // check if a vertex has more than one outgoing edge without opposite
    size_t num_vertices = m_ProcessingMesh->getNumVertices();
    
    size_t num_outgoing_without_opposite = 0;

    for(size_t i = 0; i < num_vertices; i++)
    {
        const auto& v = m_ProcessingMesh->getHESVertex(i);

        num_outgoing_without_opposite = 0;

        for(const auto& e : v->getEdges())
        {
            if(e->opposite() == nullptr)
                num_outgoing_without_opposite++;

            if(num_outgoing_without_opposite <= 1)
                continue;

            m_PartsConnectingVertices.insert(v);

            break;
        }
    }
}

void HESCheck::splitPartsConnectedInOneVertex()
{
    // variables
    HESEdge* curr_edge;
    HESEdge* prev_edge;
    HESVertex* from_vert;
    HESVertex* new_vertex;
    HESFace* face;
    std::vector<size_t> new_face_vert_idxs;
    size_t new_vert_idx;

    // for each vertex -> get edges with opposite == nullptr
    HESEdgeConstVector edge_with_null_opposite;
    
    for(const auto& v : m_PartsConnectingVertices)
    {
        // clear edge list
        edge_with_null_opposite.clear();

        // get edges at vertex with null opposite (border edges)
        for(const auto& e : v->getEdges())
            if(e->opposite() == nullptr)
                edge_with_null_opposite.push_back(e);

        // for the last num - 1 edges, create a new vertex and reconnectd neighbour elements, the first edge keeps his vertex
        for(HESEdgeConstVector::const_iterator iter = edge_with_null_opposite.begin() + 1; iter != edge_with_null_opposite.end(); ++iter)
        {
            // get elements
            curr_edge = *iter;
            prev_edge = curr_edge->prev();
            from_vert = curr_edge->from();
            face = curr_edge->face();
            new_vert_idx = m_ProcessingMesh->getNumVertices();
            
            const std::vector<size_t>& face_vert_idxs = face->getVertIds();
            new_face_vert_idxs.clear();

            // create new vertex
            m_ProcessingMesh->addVertex(*from_vert);

            // get new vertex
            new_vertex = m_ProcessingMesh->getHESVertex(new_vert_idx);

            // reconnect vertex to current and previous edge
            curr_edge->setFrom(new_vertex);
            prev_edge->setTo(new_vertex);

            // move curr edge to new vertex
            from_vert->removeEdge(curr_edge);
            new_vertex->addEdge(curr_edge);

            // create new vertex idx list for face
            for(const auto& idx : face_vert_idxs)
            {
                if(m_ProcessingMesh->getHESVertex(idx) != from_vert)
                    new_face_vert_idxs.push_back(idx);
                else
                    new_face_vert_idxs.push_back(new_vert_idx);
            }

            // set vertex idx list in face
            face->setVertIdx(new_face_vert_idxs);
        }
    }
}

void HESCheck::findBoundaries()
{
    HESEdge* e;
    bool error = false;
    size_t num_edges = m_ProcessingMesh->getNumEdges();

    HESEdgeConstVector visited_edges;

    // iterate over all edges
    for(size_t i = 0; i < num_edges; i++)
    {
        e = m_ProcessingMesh->getHESEdge(i);

        // if visited -> continue
        if(e->visited())
            continue;

        // if edge has opposit (edge is no border) -> continue
        if(e->opposite() != nullptr)
            continue;

        // set visited and push to visited
        e->setVisited(true);
        visited_edges.push_back(e);

        // find boundary (edges with same border) with boundary edge
        HESEdgeConstVector edge_loop = walkBoundary(e);

        // set found edges to visited
        for(const auto& e_loop : edge_loop)
        {
            e_loop->setVisited(true);
            visited_edges.push_back(e_loop);
        }

        // if edge loop is empty -> some error must have occured, mesh is dirty -> break
        if(edge_loop.empty())
        {
            error = true;

            break;
        }

        // add boundary to boundary list
        m_Boundaries.insert(std::make_pair(new HESFace, edge_loop));
    }

    // if error occured -> clear boundary list
    if(error)
    {
        for(const auto& b : m_Boundaries)
            delete b.first;

        m_Error = E_HESCE_BOUNDARYSEARCH;
    }

    // reset visited stated in visited edges
    for(const auto& e_visited : visited_edges)
        e_visited->setVisited(false);
}

HESCheck::HESEdgeConstVector HESCheck::walkBoundary(HESEdge* const edge)
{
    // if edge has a opposite, it is no boundary edge -> return nothing
    if(edge->opposite() != nullptr)
        return HESEdgeConstVector();

    // set edge to process
    HESEdge* e = edge;
    HESEdgeConstVector boundary;

    // add edge to boundary
    boundary.push_back(e);

    // set start edge, need to be compared while looping through
    HESEdge* const e_start = e;
    bool loop_closed = false;
    bool found_next_edge = false;

    // while loop is not closed (not found e_start)
    while(!loop_closed)
    {
        found_next_edge = false;

        // iterate over all outgoing edges of the to vertex of the current edge
        for(const auto& e_out : e->to()->getEdges())
        {
            // if we found the starting edge -> loop is closed, nothing to process anymore
            if(e_out == e_start)
            {
                loop_closed = true;

                break;
            }

            // if current outgoing edge is no border edge (opposite is not null) -> continue
            if(e_out->opposite() != nullptr)
                continue;

            // we found the next border edge
            /// set current edge to e and remind us that we found the next border edge -> break
            e = e_out;
            found_next_edge = true;

            break;
        }

        // if lopp is closed -> nothing to process here -> break
        if(loop_closed)
            break;

        // if we did not found a next edge -> mesh is dirty -> break
        if(!found_next_edge)
            break;

        // add current e to boundary
        boundary.push_back(e);
    }

    // if did not we found a boundary -> return nothing
    if(!loop_closed)
        return HESEdgeConstVector();

    // return boundary
    return boundary;
}

void HESCheck::findParts()
{
    // assign boundary edges to faces
    for(const auto& b : m_Boundaries)
    {
        for(const auto& e : b.second)
        {
            HESEdge* e_opp = new HESEdge();

            e_opp->setFrom(e->to());
            e_opp->setTo(e->from());

            e->setOpposite(e_opp);
            e_opp->setOpposite(e);

            b.first->addEdge(e_opp);

            e_opp->setFace(b.first);

            // mark e_opp as visited aka is boundary
            e_opp->setVisited(true);
        }
    }

    // walk faces and check if we found another boundary (check for mesh with hole)
    HESFaceVector visited_faces;
    HESFaceDeque next_to_visit;

    for(const auto& b : m_Boundaries)
    {
        HESEdge* start_edge = *b.second.begin();
        HESEdge* boundary_edge = start_edge->opposite();

        HESFace* boundary_face = boundary_edge->face();

        if(boundary_face->visited())
            continue;

        m_MeshParts.push_back(MeshPart());
        MeshPart& mesh_part = m_MeshParts.back();

        HESEdge* edge_opp;
        HESFace* face_opp;
        HESFace* curr_face = start_edge->face();

        // set boundary face to visited
        boundary_face->setVisited(true);

        next_to_visit.push_back(curr_face);

        while(!next_to_visit.empty())
        {
            // get next face
            curr_face = next_to_visit.front();
            next_to_visit.pop_front();

            // if we had visted the face -> continue
            if(curr_face->visited())
                continue;

            mesh_part.m_Faces.push_back(curr_face);

            // set visited and add to visited vector
            curr_face->setVisited(true);
            visited_faces.push_back(curr_face);

            // get faces arround curr_face
            for(const auto& e_face : curr_face->getEdges())
            {
                edge_opp = e_face->opposite();
                face_opp = edge_opp->face();

                // if edge_opp is visited (is marked as boundary edge) and
                // if face_opp is not the current boundary face (it is another face) and
                // if we had not visited it, yet
                // -> inc num holes in part and set new boundary edge as visited
                if(edge_opp->visited() && face_opp != boundary_face && !face_opp->visited())
                {
                    mesh_part.m_NumHoles++;
                    face_opp->setVisited(true);
                }

                // if face was not visited -> add to next to visit
                if(!face_opp->visited())
                    next_to_visit.push_back(face_opp);
            }
        }
    }

    // clear visited faces
    for(const auto& f : visited_faces)
        f->setVisited(false);

    // clear boundaries
    for(const auto& b : m_Boundaries)
    {
        for(const auto& e : b.second)
        {
            delete e->opposite();
            e->setOpposite(nullptr);
        }

        delete b.first;
    };

    // set error if we had not found a single mesh part
    if(m_MeshParts.empty())
        m_Error = E_HESCE_SPLIT;
}

void HESCheck::createMeshesFromParts()
{
    size_t num_vertices = m_ProcessingMesh->getNumVertices();
    ReIndexVector reindex_vector(num_vertices);

    for(size_t i = 0; i < num_vertices; i++)
        reindex_vector[i] = ReIndexVertex(m_ProcessingMesh->getHESVertex(i));

    // crate meshes
    for(const auto& p : m_MeshParts)
        m_Meshes.push_back(new HESMesh());

    // init add reindex map
    for(size_t i = 0; i < m_MeshParts.size(); i++)
    {
        for(const auto& f : m_MeshParts[i].m_Faces)
            for(const auto& idx : f->getVertIds())
            {
                reindex_vector[idx].m_Add = true;
                reindex_vector[idx].m_Part = i;
            }
    }

    // add vertices and set reindex
    HESMesh* current_mesh;
    for(auto& r : reindex_vector)
        if(r.m_Add)
        {
            current_mesh = m_Meshes[r.m_Part];

            r.m_Index = current_mesh->getNumVertices();

            current_mesh->addVertex(*r.m_Vertex);
        }

    // add faces
    for(size_t i = 0; i < m_MeshParts.size(); i++)
    {
        current_mesh = m_Meshes[i];

        for(const auto& f : m_MeshParts[i].m_Faces)
        {
            std::vector<size_t> new_idxs;
            const std::vector<size_t>& idxs = f->getVertIds();

            for(size_t j = 0; j < idxs.size(); j++)
                new_idxs.push_back(reindex_vector[idxs[j]].m_Index);

            current_mesh->addFace(new_idxs);
        }
    }

    // build meshes
    for(const auto& m : m_Meshes)
        HESBuilder(m).build();
}
