#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/HalfEdge/HESVertex.h"

HESMesh::HESMesh()
{

}

HESMesh::~HESMesh()
{

}

HESVertex* HESMesh::getHESVertix(const size_t& idx) const
{
    return dynamic_cast<HESVertex*>(getVertex(idx));
}

void HESMesh::addVertex(const Vec2f& pos, const Vec2f& normal)
{
    Vertex* new_vertex = new HESVertex();
    new_vertex->setPosition(pos);
    new_vertex->setNormal(normal);

    Mesh::addVertex(new_vertex);
}
