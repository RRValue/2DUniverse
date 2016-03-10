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
        HESVertex* const v0 = m_Mesh->getHESVertex(tris->data()[0]);
        HESVertex* const v1 = m_Mesh->getHESVertex(tris->data()[1]);
        HESVertex* const v2 = m_Mesh->getHESVertex(tris->data()[2]);

        HESFace* f = new HESFace();

        HESEdge* e0 = new HESEdge();
        HESEdge* e1 = new HESEdge();
        HESEdge* e2 = new HESEdge();

        // on edges
        //// set prev and next
        e0->setNext(e1);
        e1->setNext(e2);
        e2->setNext(e0);

        e0->setPrev(e2);
        e1->setPrev(e0);
        e2->setPrev(e1);

        // set faces
        e0->setFace(f);
        e1->setFace(f);
        e2->setFace(f);

        // set from and to vertices
        e0->setFrom(v0);
        e1->setFrom(v1);
        e2->setFrom(v2);

        e0->setTo(v1);
        e1->setTo(v2);
        e2->setTo(v0);

        // on faces
        //// set bound edges
        f->addEdge(e0);
        f->addEdge(e1);
        f->addEdge(e2);

        // on vertices
        //// set outgoing edges
        v0->addEdge(e0);
        v1->addEdge(e1);
        v2->addEdge(e2);

        ////////////////////////

        // check if one of the edges of to has from from
        for(const auto& e : {e0, e1, e2})
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

    return true;
}
