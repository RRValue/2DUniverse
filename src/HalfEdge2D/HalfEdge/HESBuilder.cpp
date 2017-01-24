#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

HESBuilder::HESBuilder() : m_Mesh(nullptr)
{

}

HESBuilder::HESBuilder(HESMesh* const mesh) : m_Mesh(mesh)
{

}

HESBuilder::~HESBuilder()
{

}

bool HESBuilder::build(HESMesh* const mesh)
{
    m_Mesh = mesh;

    return build();
}

bool HESBuilder::build()
{
    if(m_Mesh == nullptr)
        return false;

    // iterate over all Triangles and build basis triangles
    for(const auto& f : m_Mesh->getFaces())
    {
        const std::vector<size_t> face_idxs = f->getVertIds();
        std::vector<HESVertex*> vertices;

        for(const auto& idx : face_idxs)
            vertices.push_back(m_Mesh->getHESVertex(idx));

        std::vector<HESEdge*> edges(vertices.size());

        for(auto& e : edges)
        {
            e = new HESEdge();
            m_Mesh->addEdge(e);
        }
        
        buildFace(vertices, edges, dynamic_cast<HESFace*>(f));
        connectEdges(edges);
    }

    return true;
}

void HESBuilder::buildFace(const std::vector<HESVertex*>& vertices, const std::vector<HESEdge*>& edges, HESFace* const face)
{
    size_t num_vertices = vertices.size();

    // on edges
    //// set prev and next
    for(size_t i = 0; i < num_vertices; i++)
    {
        edges[i]->setNext(edges[(i + 1) % num_vertices]);
        edges[i]->setPrev(edges[(i - 1 + num_vertices) % num_vertices]);
    }

    // set faces
    for(const auto& e : edges)
        e->setFace(face);

    // set from and to vertices
    for(size_t i = 0; i < num_vertices; i++)
    {
        edges[i]->setFrom(vertices[i]);
        edges[i]->setTo(vertices[(i + 1) % num_vertices]);
    }

    // on faces
    //// set bound edges
    for(const auto& e : edges)
        face->addEdge(e);
    
    // on vertices
    //// set outgoing edges
    for(size_t i = 0; i < num_vertices; i++)
        vertices[i]->addEdge(edges[i]);
}

void HESBuilder::connectEdges(const std::vector<HESEdge*>& edges)
{
    // check if one of the edges of to has from from
    for(const auto& e : edges)
    {
        HESVertex* const org_from = e->from();
        HESVertex* const org_to = e->to();

        HESEdge* edge_curre = e;
        HESEdge* edge_found = nullptr;

        bool found = false;

        for(const auto& outgoing : org_to->getEdges())
        {
            if(outgoing->to() != org_from)
                continue;

            edge_found = outgoing;
            found = true;

            break;
        }

        if(!found)
            continue;

        edge_curre->setOpposite(edge_found);
        edge_found->setOpposite(edge_curre);
    }
}
