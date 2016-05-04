#include "HalfEdge2D/HalfEdge/HESCheck.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

HESCheck::HESCheck(HESMesh* const mesh) : m_SourceMesh(mesh)
{
    m_HasPartConnectedInOneVertex = false;
    m_HasParts = false;
    
    run();
}

HESCheck::~HESCheck()
{
    delete m_ProcessingMesh;
}

void HESCheck::run()
{
    m_ProcessingMesh = new HESMesh(*m_SourceMesh);

    // check for connected parts in one vertex
    checkHasPartsConnectedInOneVertex();

    if(m_HasPartConnectedInOneVertex)
        splitPartsConnectedInOneVertex();

    // check for unconnected parts
    splitParts();
}

void HESCheck::checkHasPartsConnectedInOneVertex()
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

    if(m_PartsConnectingVertices.empty())
        return;

    m_HasPartConnectedInOneVertex = true;
    m_HasParts = true;
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

void HESCheck::splitParts()
{
    // find boundary circles
    std::vector<HESEdgeConstVector> boundaries;

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
        boundaries.push_back(edge_loop);
    }

    // if error occured -> clear boundary list
    if(error)
        boundaries.clear();

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

    // while loop is not closed (found e_start)
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
