#ifndef _RENDERABLE_SPLINE_H_
#define _RENDERABLE_SPLINE_H_

#include "2DUniverse/Base/Spline.h"

#include "2DUniverse/Renderables/RenderableLine.h"
#include "2DUniverse/Renderables/Intersectable.h"

class Spline : public Spline2D, public RenderableLine, public Intersectable
{
public:
    Spline();
    Spline(const Spline& other);
    Spline(Spline&& other);
    ~Spline();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

public:
    virtual IntersectionVector intersect(Circle* const c, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Line* const l, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(QuadraticBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(CubicBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Spline* const s, const bool& valuesFromIntersector = false) const final override;
};

#endif //_RENDERABLE_SPLINE_H_
