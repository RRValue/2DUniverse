#include "HalfEdge2D/Renderables/Circle.h"

Circle::Circle()
{
    m_Radius = 0.1f;
    m_Position = Vec2f(0.0f, 0.0f);
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_Visible = true;
}

Circle::~Circle()
{

}

const float& Circle::getRadius() const
{
    return m_Radius;
}

const Vec2f& Circle::getPosition() const
{
    return m_Position;
}

const Vec4f& Circle::getColour() const
{
    return m_Colour;
}

const bool& Circle::isVisible() const
{
    return m_Visible;
}

void Circle::setRadius(const float& radius)
{
    m_Radius = radius;
}

void Circle::setPosition(const Vec2f& pos)
{
    m_Position = pos;
}

void Circle::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}

void Circle::setVisible(const bool& visible)
{
    m_Visible = visible;
}
