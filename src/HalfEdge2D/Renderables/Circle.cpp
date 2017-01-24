#include "HalfEdge2D/Renderables/Circle.h"

#include "HalfEdge2D/Renderables/Line.h"

Circle::Circle()
{
    m_Radius = 0.1f;
    m_Position = Vec2d(0.0, 0.0);
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

const Vec2d& Circle::getPosition() const
{
    return m_Position;
}

void Circle::setRadius(const float& radius)
{
    m_Radius = radius;
}

void Circle::setPosition(const Vec2d& pos)
{
    m_Position = pos;
}

void Circle::transform(const Mat3d& mat)
{
    Vec3d c_t = mat * Vec3d(m_Position[0], m_Position[1], 1.0);

    m_Position(0) = c_t(0);
    m_Position(1) = c_t(1);
}

IntersectionVector Circle::intersect(Circle* const c, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Circle with Circle not impl.");

    return IntersectionVector();
}

IntersectionVector Circle::intersect(Line* const l, const bool& valuesFromIntersector) const
{
    return ((Intersectable*)l)->intersect(const_cast<Circle*>(this), true);
}
IntersectionVector Circle::intersect(QuadraticBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Circle with QuadraticBezier not impl.");

    return IntersectionVector();
}

IntersectionVector Circle::intersect(CubicBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Circle with CubicBezier not impl.");

    return IntersectionVector();
}

IntersectionVector Circle::intersect(Spline* const s, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Circle with Spline not impl.");

    return IntersectionVector();
}
