#include "HalfEdge2D/Mesh/Vertex.h"

Vertex::Vertex() :
m_Position(Vec2f(0.0f, 0.0f)),
m_Normal(Vec2f(0.0f, 0.0f))
{

}

Vertex::~Vertex()
{

}

const Vec2f& Vertex::getPosition() const
{
    return m_Position;
}

const Vec2f& Vertex::getNormal() const
{
    return m_Normal;
}

void Vertex::setPosition(const Vec2f& position)
{
    m_Position = position;
}

void Vertex::setNormal(const Vec2f& normal)
{
    m_Normal = normal;
}
