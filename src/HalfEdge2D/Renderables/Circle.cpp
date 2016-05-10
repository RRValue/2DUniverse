#include "HalfEdge2D/Renderables/Circle.h"

#include "HalfEdge2D/Renderables/Line.h"

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

void Circle::transform(const Mat3f& mat)
{
    Vec3f c_t = mat * Vec3f(m_Position[0], m_Position[1], 1.0);

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
    return ((Intersectable*)l)->intersect(const_cast<Circle* const>(this), true);
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
