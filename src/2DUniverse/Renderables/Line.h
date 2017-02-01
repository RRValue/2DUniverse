#ifndef _RENDERABLE_LINE_H_
#define _RENDERABLE_LINE_H_

#include "2DUniverse/Base/Vector.h"
#include "2DUniverse/Base/NGradeBezier.h"

#include "2DUniverse/Renderables/RenderableLine.h"
#include "2DUniverse/Renderables/Intersectable.h"

#include <vector>

class Line : public Line2D, public RenderableLine, public Intersectable
{
public:
    Line();
    Line(const Line& other);
    Line(Line&& other);
    ~Line();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    
    // tooling
    bool collinearTo(const Line& l) const;
    double getLength() const;
    Vec2d getNormal() const;

public:
    virtual IntersectionVector intersect(Circle* const c, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Line* const l, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(QuadraticBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(CubicBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Spline* const s, const bool& valuesFromIntersector = false) const final override;

private:
    Mat3d getOrthoBaseMatrix() const;
};

#endif //_RENDERABLE_LINE_H_
