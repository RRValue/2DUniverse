#include "2DUniverse/Renderables/QuadraticBezier.h"

#include "2DUniverse/Renderables/Line.h"

QuadraticBezier::QuadraticBezier()
{

}

QuadraticBezier::QuadraticBezier(const QuadraticBezier& other) : QBezier2D(other), RenderableLine(other)
{

}

QuadraticBezier::QuadraticBezier(QuadraticBezier&& other) : QBezier2D(std::move(other)), RenderableLine(std::move(other))
{

}

QuadraticBezier::~QuadraticBezier()
{

}

IntersectionVector QuadraticBezier::intersect(Circle* const c, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection QuadraticBezier with Circle not impl.");

    return IntersectionVector();
}

IntersectionVector QuadraticBezier::intersect(Line* const l, const bool& valuesFromIntersector) const
{
    return l->intersect(const_cast<QuadraticBezier*>(this), !valuesFromIntersector);
}
IntersectionVector QuadraticBezier::intersect(QuadraticBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection QuadraticBezier with QuadraticBezier not impl.");

    return IntersectionVector();
}

IntersectionVector QuadraticBezier::intersect(CubicBezier* const b, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection QuadraticBezier with CubicBezier not impl.");

    return IntersectionVector();
}

IntersectionVector QuadraticBezier::intersect(Spline* const s, const bool& valuesFromIntersector) const
{
    assert(false && "Intersection QuadraticBezier with Spline not impl.");

    return IntersectionVector();
}
