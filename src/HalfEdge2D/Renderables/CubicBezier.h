#ifndef _RENDERABLE_CUBICBEZIER_H_
#define _RENDERABLE_CUBICBEZIER_H_

#include "HalfEdge2D/Base/NGradeBezier.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"
#include "HalfEdge2D/Renderables/Intersectable.h"

class CubicBezier : public CBezier2F, public RenderableLine, public Intersectable
{
public:
    CubicBezier();
    CubicBezier(const CubicBezier& other);
    CubicBezier(CubicBezier&& other);
    ~CubicBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
    virtual IntersectionVector intersect(Circle* const c, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Line* const l, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(QuadraticBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(CubicBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Spline* const s, const bool& valuesFromIntersector = false) const final override;
};

#endif //_RENDERABLE_CUBICBEZIER_H_
