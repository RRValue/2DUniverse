#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/HalfEdge/HESVertex.h"

HESMesh::HESMesh()
{
    m_Changed = false;
}

HESMesh::~HESMesh()
{

}

HESVertex* HESMesh::getHESVertix(const size_t& idx) const
{
    return dynamic_cast<HESVertex*>(getVertex(idx));
}

const bool& HESMesh::hasChanged() const
{
    return m_Changed;
}

Vertex* HESMesh::allocateVertex()
{
    return new HESVertex();
}
