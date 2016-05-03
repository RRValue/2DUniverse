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
