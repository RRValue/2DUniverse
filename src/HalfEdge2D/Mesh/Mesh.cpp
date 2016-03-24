#include "HalfEdge2D/Mesh/Mesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Face.h"

#include <cassert>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    clear();
}

void Mesh::clear()
{
    for(const auto& vertex : m_Vertices)
        delete vertex;

    for(const auto& face : m_Faces)
        delete face;

    m_Vertices.clear();
    m_Faces.clear();
}

const std::vector<Vertex*>& Mesh::getVertices() const
{
    return m_Vertices;
}

const std::vector<Face*>& Mesh::getFaces() const
{
    return m_Faces;
}

Vertex* const Mesh::getVertex(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Vertices.size());

    return m_Vertices[idx];
}

Face* const Mesh::getFace(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Faces.size());

    return m_Faces[idx];
}

Vertex* Mesh::allocateVertex()
{
    return new Vertex();
}

Face* Mesh::allocateFace()
{
    return new Face();
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

void Mesh::addFace(const size_t& idx0, const size_t& idx1, const size_t& idx2)
{
    Face* new_face = allocateFace();
    new_face->addVertIdx({idx0, idx1, idx2});

    m_Faces.push_back(new_face);
}

void Mesh::addFace(const std::initializer_list<size_t>& idxs)
{
    Face* new_face = allocateFace();
    new_face->addVertIdx(idxs);

    m_Faces.push_back(new_face);
}

void Mesh::removeFace(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Faces.size());

    std::vector<Face*>::iterator iter = m_Faces.begin() + idx;

    delete *iter;

    m_Faces.erase(iter);
}
