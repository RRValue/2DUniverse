#include "2DUniverse/Renderables/Spline.h"

#include "2DUniverse/Renderables/Line.h"

#include <cassert>

Spline::Spline()
{

}

Spline::Spline(const Spline& other) : Spline2D(other), RenderableLine(other)
{

}

Spline::Spline(Spline&& other) : Spline2D(std::move(other)), RenderableLine(std::move(other))
{

}

Spline::~Spline()
{

}

IntersectionVector Spline::intersect(Circle* const c, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Spline with Circle not impl.");

    return IntersectionVector();
}

IntersectionVector Spline::intersect(Line* const l, const bool& valuesFromIntersector) const
{
    return l->intersect(const_cast<Spline*>(this), !valuesFromIntersector);
}
IntersectionVector Spline::intersect(QuadraticBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Spline with QuadraticBezier not impl.");

    return IntersectionVector();
}

IntersectionVector Spline::intersect(CubicBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Spline with CubicBezier not impl.");

    return IntersectionVector();
}

IntersectionVector Spline::intersect(Spline* const s, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection Spline with Spline not impl.");

    return IntersectionVector();
}
