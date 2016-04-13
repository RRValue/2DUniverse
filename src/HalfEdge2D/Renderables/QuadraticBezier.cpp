#include "HalfEdge2D/Renderables/QuadraticBezier.h"

QuadraticBezier::QuadraticBezier()
{
    m_Thickness = 0.01f;
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_Visible = true;

    m_Points[0] = m_Points[1] = m_Points[2] = Vec2f(0.0f, 0.0f);
}

QuadraticBezier::~QuadraticBezier()
{

}

const float& QuadraticBezier::getThickness() const
{
    return m_Thickness;
}

const std::array<Vec2f, 3>& QuadraticBezier::getPoints() const
{
    return m_Points;
}

const Vec2f& QuadraticBezier::getPoint(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Points.size() && "idx out of range");

    return m_Points[idx];
}

const Vec4f& QuadraticBezier::getColour() const
{
    return m_Colour;
}

const bool& QuadraticBezier::isVisible() const
{
    return m_Visible;
}

Vec2f QuadraticBezier::pointAt(const float& alpha)
{
    return blend(m_Points, alpha);
}

void QuadraticBezier::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}

void QuadraticBezier::setPoint(const size_t& idx, const Vec2f& point)
{
    assert(idx >= 0 && idx < m_Points.size() && "idx out of range");

    m_Points[idx] = point;
}

void QuadraticBezier::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}

void QuadraticBezier::setVisible(const bool& visible)
{
    m_Visible = visible;
}
