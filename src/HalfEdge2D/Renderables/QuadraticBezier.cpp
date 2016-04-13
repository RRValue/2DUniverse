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

Vec2f QuadraticBezier::pointAt(const float& alpha) const
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

void QuadraticBezier::transform(const Mat3f& m)
{
    for(auto& p : m_Points)
    {
        Vec3f p_t = m * Vec3f(p[0], p[1], 1.0f);
        p = Vec2f(p_t[0], p_t[1]);
    }
}

QuadraticBezier QuadraticBezier::transformed(const Mat3f& m) const
{
    QuadraticBezier b;

    for(size_t i = 0; i < m_Points.size(); i++)
    {
        Vec3f p_t = m * Vec3f(m_Points[i][0], m_Points[i][1], 1.0f);
        b.setPoint(i, Vec2f(p_t[0], p_t[1]));
    }

    return b;
}

std::vector<float> QuadraticBezier::rootsX() const
{
    Result solution = solve({&(float)m_Points[0][0], &(float)m_Points[1][0], &(float)m_Points[2][0]});
    std::vector<float> roots;

    for(size_t i = 0; i < solution.m_Solutions; i++)
        roots.push_back(solution[0]);

    return roots;
}

std::vector<float> QuadraticBezier::rootsY() const
{
    Result solution = solve({&(float)m_Points[0][1], &(float)m_Points[1][1], &(float)m_Points[2][1]});
    std::vector<float> roots;

    for(size_t i = 0; i < solution.m_Solutions; i++)
        roots.push_back(solution[0]);

    return roots;
}
