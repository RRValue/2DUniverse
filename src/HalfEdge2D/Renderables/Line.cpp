#include "HalfEdge2D/Renderables/Line.h"

Line::Line()
{
    m_Thickness = 0.01f;
    m_PositionStart = Vec2f(-1.0f, 0.0f);
    m_PositionEnd = Vec2f(1.0f, 0.0f);
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
}

Line::~Line()
{

}

const float& Line::getThickness() const
{
    return m_Thickness;
}

const Vec2f& Line::getPositionStart() const
{
    return m_PositionStart;
}

const Vec2f& Line::getPositionEnd() const
{
    return m_PositionEnd;
}

const Vec4f& Line::getColour() const
{
    return m_Colour;
}

void Line::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}

void Line::setPositionStart(const Vec2f& pos)
{
    m_PositionStart = pos;
}

void Line::setPositionEnd(const Vec2f& pos)
{
    m_PositionEnd = pos;
}

void Line::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}
