#include "2DUniverse/Mesh/Vertex.h"

Vertex::Vertex() :
    m_Position(Vec2d(0.0, 0.0)),
    m_Normal(Vec2d(0.0, 0.0)),
    m_Colour(Vec4f(0.0f, 0.0f, 0.0f, 1.0f))
{

}

Vertex::Vertex(const Vertex& other)
{
    m_Position = other.m_Position;
    m_Normal = other.m_Normal;
    m_Colour = other.m_Colour;
}

Vertex::~Vertex()
{

}

const Vec2d& Vertex::getPosition() const
{
    return m_Position;
}

const Vec2d& Vertex::getNormal() const
{
    return m_Normal;
}

const Vec4f& Vertex::getColour() const
{
    return m_Colour;
}

void Vertex::setPosition(const Vec2d& position)
{
    m_Position = position;
}

void Vertex::setNormal(const Vec2d& normal)
{
    m_Normal = normal;
}

void Vertex::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}
