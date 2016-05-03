#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Renderables/Renderable.h"

#include "HalfEdge2D/Mesh/Vertex.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

HESMesh::HESMesh()
{
    m_Changed = false;
}

HESMesh::HESMesh(const HESMesh& other)
{
    *this = other;
}

HESMesh::~HESMesh()
{

}

void HESMesh::clear()
{
    for(const auto& edge : m_Edges)
        delete edge;

    m_Edges.clear();

    Mesh::clear();
}

size_t HESMesh::getNumEdges() const
{
    return m_Edges.size();
}

HESEdge* HESMesh::getHESEdge(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Edges.size());

    return m_Edges[idx];
}

HESVertex* HESMesh::getHESVertex(const size_t& idx) const
{
    return dynamic_cast<HESVertex*>(getVertex(idx));
}

HESFace* HESMesh::getHESFace(const size_t& idx) const
{
    return dynamic_cast<HESFace*>(getFace(idx));
}

const bool& HESMesh::hasChanged() const
{
    return m_Changed;
}

void HESMesh::addEdge(HESEdge* const edge)
{
    m_Edges.push_back(edge);
}

Vertex* HESMesh::allocateVertex()
{
    return new HESVertex();
}

Face* HESMesh::allocateFace()
{
    return new HESFace();
}

HESMesh& HESMesh::operator=(const HESMesh& other)
{
    clear();

    m_Changed = other.m_Changed;

    for(const auto& v : other.m_Vertices)
        addVertex(*v);

    for(const auto& f : other.m_Faces)
        addFace(*f);

    HESBuilder(this).build();

    return *this;
}
