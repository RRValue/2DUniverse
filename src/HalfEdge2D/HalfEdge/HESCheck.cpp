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

}

void HESCheck::run()
{
    m_ProcessingMesh = new HESMesh(*m_SourceMesh);

    // check for unconnected parts
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

            new_vertex->addEdge(curr_edge);

            from_vert->removeEdge(curr_edge);

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
    std::vector<std::vector<HESEdge*>> boundaries;

    HESEdge* e_start;
    HESEdge* e;
    bool loop_closed;
    bool error_find_loop = false;
    bool found_next_edge;
    size_t num_edges = m_ProcessingMesh->getNumEdges();
    
    for(size_t i = 0; i < num_edges; i++)
    {
        e = m_ProcessingMesh->getHESEdge(i);

        if(e->visited())
            continue;

        if(e->opposite() == nullptr)
        {
            std::vector<HESEdge*> boundary;

            boundary.push_back(e);
            e->setVisited(true);

            e_start = e;
            loop_closed = false;

            while(!loop_closed)
            {
                found_next_edge = false;

                for(const auto& e_out : e->to()->getEdges())
                {
                    if(e_out == e_start)
                        loop_closed = true;

                    if(loop_closed)
                        break;

                    if(e_out == e)
                        continue;

                    e_out->setVisited(true);

                    if(e_out->opposite() != nullptr)
                        continue;

                    e = e_out;
                    found_next_edge = true;

                    break;
                }

                if(loop_closed)
                    break;

                if(!found_next_edge)
                    break;

                boundary.push_back(e);
            }

            if(loop_closed)
            {
                boundaries.push_back(boundary);
                boundary.clear();

                continue;
            }

            if(!found_next_edge)
                error_find_loop = true;
        }

        if(error_find_loop)
            break;
    }

    if(error_find_loop)
        boundaries.clear();

    // reset visited
    for(size_t i = 0; i < num_edges; i++)
        m_ProcessingMesh->getHESEdge(i)->setVisited(false);
}
