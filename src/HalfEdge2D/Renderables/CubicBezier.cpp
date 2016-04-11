#include "HalfEdge2D/Renderables/CubicBezier.h"

CubicBezier::CubicBezier()
{
    m_Colour= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    m_Visible = true;

    m_Points[0] = m_Points[1] = m_Points[2] = m_Points[3] = Vec2f(0.0f, 0.0f);
}

CubicBezier::~CubicBezier()
{

}

const std::array<Vec2f, 4>& CubicBezier::getPoints() const
{
    return m_Points;
}

const Vec2f& CubicBezier::getPoint(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Points.size() && "idx out of range");

    return m_Points[idx];
}

const Vec4f& CubicBezier::getColour() const
{
    return m_Colour;
}

const bool& CubicBezier::isVisible() const
{
    return m_Visible;
}

Vec2f CubicBezier::pointAt(const float& alpha)
{
    return blend(m_Points, alpha);
}

void CubicBezier::setPoint(const size_t& idx, const Vec2f& point)
{
    assert(idx >= 0 && idx < m_Points.size() && "idx out of range");

    m_Points[idx] = point;
}

void CubicBezier::setColour(const Vec4f& colour)
{
    m_Colour = colour;
}

void CubicBezier::setVisible(const bool& visible)
{
    m_Visible = visible;
}
