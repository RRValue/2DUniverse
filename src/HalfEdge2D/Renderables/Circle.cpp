#include "HalfEdge2D/Renderables/Circle.h"

Circle::Circle()
{
    m_Radius = 0.1f;
    m_Position = Vec2f(0.0f, 0.0f);
}

Circle::Circle(const Circle& other) : RenderableLine(other), m_Radius(other.m_Radius), m_Position(other.m_Position)
{

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

void Circle::setRadius(const float& radius)
{
    m_Radius = radius;
}

void Circle::setPosition(const Vec2f& pos)
{
    m_Position = pos;
}
