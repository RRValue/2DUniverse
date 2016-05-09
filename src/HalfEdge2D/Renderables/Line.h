#ifndef _RENDERABLE_LINE_H_
#define _RENDERABLE_LINE_H_

#include "HalfEdge2D/Base/Vector.h"
#include "HalfEdge2D/Base/NGradeBezier.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"

#include "HalfEdge2D/Renderables/Circle.h"

#include "HalfEdge2D/Renderables/QuadraticBezier.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"
#include "HalfEdge2D/Renderables/Spline.h"

#include <vector>

class Line : public Line2F, public RenderableLine
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
    
    Vec2fVector intersect(const Circle& c) const;
    Vec2fVector intersect(const Line& l) const;
    Vec2fVector intersect(const QuadraticBezier& b) const;
    Vec2fVector intersect(const CubicBezier& b) const;
    Vec2fVector intersect(const Spline& s) const;

private:
    Mat3f getOrthoBaseMatrix() const;
};

#endif //_RENDERABLE_LINE_H_
