#include "HalfEdge2D/Renderables/CubicBezier.h"

#include "HalfEdge2D/Renderables/Line.h"

CubicBezier::CubicBezier()
{

}

CubicBezier::CubicBezier(const CubicBezier& other) : CBezier2D(other), RenderableLine(other)
{

}

CubicBezier::CubicBezier(CubicBezier&& other) : CBezier2D(std::move(other)), RenderableLine(std::move(other))
{

}

CubicBezier::~CubicBezier()
{

}

IntersectionVector CubicBezier::intersect(Circle* const c, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection CubicBezier with Circle not impl.");

    return IntersectionVector();
}

IntersectionVector CubicBezier::intersect(Line* const l, const bool& valuesFromIntersector) const
{
    return ((Intersectable*)l)->intersect(const_cast<CubicBezier* const>(this), !valuesFromIntersector);
}
IntersectionVector CubicBezier::intersect(QuadraticBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection CubicBezier with QuadraticBezier not impl.");

    return IntersectionVector();
}

IntersectionVector CubicBezier::intersect(CubicBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection CubicBezier with CubicBezier not impl.");

    return IntersectionVector();
}

IntersectionVector CubicBezier::intersect(Spline* const s, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection CubicBezier with Spline not impl.");

    return IntersectionVector();
}
