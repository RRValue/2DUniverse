#include "HalfEdge2D/Mesh/Mesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Face.h"

#include <cassert>

Mesh::Mesh()
{

}

Mesh::Mesh(const Mesh& other) : Renderable(other)
{
    copy(other);
}

Mesh::~Mesh()
{
    clear();
}

Mesh& Mesh::operator=(const Mesh& other)
{
    clear();
    copy(other);

    return *this;
}

void Mesh::copy(const Mesh& other)
{
    // copy vertices
    size_t num_v = other.m_Vertices.size();
    m_Vertices = std::vector<Vertex*>(num_v);

    for(size_t i = 0; i < num_v; i++)
        m_Vertices[i] = new Vertex(*other.m_Vertices[i]);

    // copy faces
    size_t num_f = other.m_Faces.size();
    m_Faces = std::vector<Face*>(num_f);

    for(size_t i = 0; i < num_f; i++)
        m_Faces[i] = new Face(*other.m_Faces[i]);
}

void Mesh::clear()
{
    for(const auto& vertex : m_Vertices)
        delete vertex;

    for(const auto& face : m_Faces)
        delete face;

    m_Vertices.clear();
    m_Faces.clear();

    setDirty();
}

size_t Mesh::getNumVertices() const
{
    return m_Vertices.size();
}

size_t Mesh::getNumFaces() const
{
    return m_Faces.size();
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

    setDirty();
}

void Mesh::addVertex(const Vertex& vertex)
{
    Vertex* new_vertex = allocateVertex();
    new_vertex->setPosition(vertex.getPosition());
    new_vertex->setNormal(vertex.getNormal());
    new_vertex->setColour(vertex.getColour());

    m_Vertices.push_back(new_vertex);

    setDirty();
}

void Mesh::addVertex(const Vec2f& pos)
{
    Vertex* new_vertex = allocateVertex();
    new_vertex->setPosition(pos);
    
    m_Vertices.push_back(new_vertex);
    
    setDirty();
}

void Mesh::addVertex(const Vec2f& pos, const Vec2f& normal)
{
    Vertex* new_vertex = allocateVertex();
    new_vertex->setPosition(pos);
    new_vertex->setNormal(normal);

    m_Vertices.push_back(new_vertex);

    setDirty();
}

void Mesh::addVertex(const Vec2f& pos, const Vec2f& normal, const Vec4f& color)
{
    Vertex* new_vertex = allocateVertex();
    new_vertex->setPosition(pos);
    new_vertex->setNormal(normal);
    new_vertex->setColour(color);

    m_Vertices.push_back(new_vertex);

    setDirty();
}

void Mesh::removeVertex(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Vertices.size());

    std::vector<Vertex*>::iterator iter = m_Vertices.begin() + idx;

    delete *iter;

    m_Vertices.erase(iter);

    setDirty();
}

void Mesh::addFace(const Face& f)
{
    Face* new_face = allocateFace();
    new_face->addVertIdx(f.getVertIds());
    new_face->setColour(f.getColour());

    m_Faces.push_back(new_face);

    setDirty();
}

void Mesh::addFace(const size_t& idx0, const size_t& idx1, const size_t& idx2)
{
    Face* new_face = allocateFace();
    new_face->addVertIdx({idx0, idx1, idx2});

    m_Faces.push_back(new_face);

    setDirty();
}

void Mesh::addFace(const std::initializer_list<size_t>& idxs)
{
    Face* new_face = allocateFace();
    new_face->addVertIdx(idxs);

    m_Faces.push_back(new_face);

    setDirty();
}

void Mesh::addFace(const std::vector<size_t>& idxs)
{
    Face* new_face = allocateFace();
    new_face->addVertIdx(idxs);

    m_Faces.push_back(new_face);

    setDirty();
}

void Mesh::removeFace(const size_t& idx)
{
    assert(idx >= 0 && idx < m_Faces.size());

    std::vector<Face*>::iterator iter = m_Faces.begin() + idx;

    delete *iter;

    m_Faces.erase(iter);

    setDirty();
}

void Mesh::setDirty()
{

}
