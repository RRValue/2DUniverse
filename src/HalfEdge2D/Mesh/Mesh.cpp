#include "HalfEdge2D/Mesh/Mesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Triangle.h"

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

const std::vector<Triangle*>& Mesh::getTriangles() const
{
    return m_Triangles;
}

Vertex* const Mesh::getVertex(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Vertices.size());

    return m_Vertices[idx];
}

Triangle* const Mesh::getTriangle(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Triangles.size());

    return m_Triangles[idx];
}

Vertex* Mesh::allocateVertex()
{
    return new Vertex();
}

void Mesh::addVertex()
{
    m_Vertices.push_back(allocateVertex());
}

void Mesh::addVertex(const Vec2f& pos)
{
    Vertex* new_vertex = allocateVertex();
    new_vertex->setPosition(pos);
    
    m_Vertices.push_back(new_vertex);
}

void Mesh::addVertex(const Vec2f& pos, const Vec2f& normal)
{
    Vertex* new_vertex = allocateVertex();
    new_vertex->setPosition(pos);
    new_vertex->setNormal(normal);

    m_Vertices.push_back(new_vertex);
}

void Mesh::removeVertex(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Vertices.size());

    std::vector<Vertex*>::iterator iter = m_Vertices.begin() + idx;

    delete *iter;

    m_Vertices.erase(iter);
}

void Mesh::addTriangle(const size_t& idx0, const size_t& idx1, const size_t& idx2)
{
    Triangle* new_tris = new Triangle();
    new_tris->data()[0] = idx0;
    new_tris->data()[1] = idx1;
    new_tris->data()[2] = idx2;

    m_Triangles.push_back(new_tris);
}

void Mesh::removeTriangle(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Triangles.size());

    std::vector<Triangle*>::iterator iter = m_Triangles.begin() + idx;

    delete *iter;

    m_Triangles.erase(iter);
}
