#include "HalfEdge2D/Renderables/Point.h"

Point::Point()
{
    m_Size = 0.01f;
    m_Position = Vec2f(0.0f, 0.0f);
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
}

Point::~Point()
{

}

const float& Point::getSize() const
{
    return m_Size;
}

const Vec2f& Point::getPosition() const
{
    return m_Position;
}

const Vec4f& Point::getColour() const
{
    return m_Colour;
}

void Point::setSize(const float& size)
{
    m_Size = size;
}

void Point::setPosition(const Vec2f& pos)
{
    m_Position = pos;
}

void Point::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}
