#include "2DUniverse/Renderables/Point.h"

Point::Point()
{
    m_Size = 0.01f;
    m_Position = Vec2d(0.0, 0.0);
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_Visible = true;
}

Point::~Point()
{

}

const float& Point::getSize() const
{
    return m_Size;
}

const Vec2d& Point::getPosition() const
{
    return m_Position;
}

const Vec4f& Point::getColour() const
{
    return m_Colour;
}

const bool& Point::isVisible() const
{
    return m_Visible;
}

void Point::setSize(const float& size)
{
    m_Size = size;
}

void Point::setPosition(const Vec2d& pos)
{
    m_Position = pos;
}

void Point::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}

void Point::setVisible(const bool& visible)
{
    m_Visible = visible;
}
