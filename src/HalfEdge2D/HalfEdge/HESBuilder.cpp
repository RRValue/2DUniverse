#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include "HalfEdge2D/Mesh/Triangle.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

HESBuilder::HESBuilder(HESMesh* const mesh) : m_Mesh(mesh)
{

}

HESBuilder::~HESBuilder()
{

}

bool HESBuilder::build()
{
    if(m_Mesh == nullptr)
        return false;

    // iterate over all Triangles and build basis triangles
    for(const auto& tris : m_Mesh->getTriangles())
    {
        HESVertex* const v0 = m_Mesh->getHESVertex(tris->getIdx0());
        HESVertex* const v1 = m_Mesh->getHESVertex(tris->getIdx1());
        HESVertex* const v2 = m_Mesh->getHESVertex(tris->getIdx2());

        HESFace* f = new HESFace();

        HESEdge* e0 = new HESEdge();
        HESEdge* e1 = new HESEdge();
        HESEdge* e2 = new HESEdge();

        m_Mesh->addFace(f);
        
        m_Mesh->addEdge(e0);
        m_Mesh->addEdge(e1);
        m_Mesh->addEdge(e2);

        buildFace({v0, v1, v2}, {e0, e1, e2}, f);
        connectEdges({e0, e1, e2});
    }

    return true;
}

void HESBuilder::buildFace(const std::array<HESVertex* const, 3>& vertices, const std::array<HESEdge* const, 3>& edges, HESFace* const face)
{
    // on edges
    //// set prev and next
    edges[0]->setNext(edges[1]);
    edges[1]->setNext(edges[2]);
    edges[2]->setNext(edges[0]);

    edges[0]->setPrev(edges[2]);
    edges[1]->setPrev(edges[0]);
    edges[2]->setPrev(edges[1]);

    // set faces
    edges[0]->setFace(face);
    edges[1]->setFace(face);
    edges[2]->setFace(face);

    // set from and to vertices
    edges[0]->setFrom(vertices[0]);
    edges[1]->setFrom(vertices[1]);
    edges[2]->setFrom(vertices[2]);

    edges[0]->setTo(vertices[1]);
    edges[1]->setTo(vertices[2]);
    edges[2]->setTo(vertices[0]);

    // on faces
    //// set bound edges
    face->addEdge(edges[0]);
    face->addEdge(edges[1]);
    face->addEdge(edges[2]);

    // on vertices
    //// set outgoing edges
    vertices[0]->addEdge(edges[0]);
    vertices[1]->addEdge(edges[1]);
    vertices[2]->addEdge(edges[2]);
}

void HESBuilder::connectEdges(const std::initializer_list<HESEdge* const>& edges)
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
