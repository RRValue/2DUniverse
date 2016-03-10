#include "HalfEdge2D/Mesh/Mesh.h"
#include "HalfEdge2D/Mesh/Vertex.h"

#include <cassert>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    for(const auto& vertex : m_Vertices)
        delete vertex;
}

const std::vector<Vertex*>& Mesh::getVertices() const
{
    return m_Vertices;
}

Vertex* const Mesh::getVertex(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Vertices.size());

    return m_Vertices[idx];
}

void Mesh::addVertex(Vertex* const vertex)
{
    m_Vertices.push_back(vertex);
}

void Mesh::addVertex(const Vec2f& pos, const Vec2f& normal)
{
    Vertex* new_vertex = new Vertex();
    new_vertex->setPosition(pos);
    new_vertex->setNormal(normal);

    addVertex(new_vertex);
}

void Mesh::removeVertex(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Vertices.size());

    m_Vertices.erase(m_Vertices.begin() + idx);
}
