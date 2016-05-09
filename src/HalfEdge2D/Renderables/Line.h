#ifndef _RENDERABLE_LINE_H_
#define _RENDERABLE_LINE_H_

#include "HalfEdge2D/Base/Vector.h"
#include "HalfEdge2D/Base/NGradeBezier.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"
#include "HalfEdge2D/Renderables/Intersectable.h"

#include <vector>

class Line : public Line2F, public RenderableLine, public Intersectable
{
public:
    Line();
    Line(const Line& other);
    Line(Line&& other);
    ~Line();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    
    // tooling
    bool collinearTo(const Line& l) const;
    float getLength() const;
    Vec2f getNormal() const;

public:
    virtual IntersectionVector intersect(Circle* const circle) const final override;
    virtual IntersectionVector intersect(Line* const line) const final override;
    virtual IntersectionVector intersect(QuadraticBezier* const b) const final override;
    virtual IntersectionVector intersect(CubicBezier* const b) const final override;
    virtual IntersectionVector intersect(Spline* const s) const final override;

private:
    Mat3f getOrthoBaseMatrix() const;
};

#endif //_RENDERABLE_LINE_H_
