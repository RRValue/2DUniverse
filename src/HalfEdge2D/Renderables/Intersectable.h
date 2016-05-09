#ifndef _RENDERABLES_INTERSECTABLE_H_
#define _RENDERABLES_INTERSECTABLE_H_

#include "HalfEdge2D/Base/Vector.h"

class Circle;
class Line;
class QuadraticBezier;
class CubicBezier;
class Spline;

struct Intersection
{
    float m_Alpha;
    Vec2f m_Point;
};

typedef std::vector<Intersection, Eigen::aligned_allocator<Intersection>> IntersectionVector;

class Intersectable
{
public:
    virtual IntersectionVector intersect(Circle* const circle) const = 0;
    virtual IntersectionVector intersect(Line* const l) const = 0;
    virtual IntersectionVector intersect(QuadraticBezier* const b) const = 0;
    virtual IntersectionVector intersect(CubicBezier* const b) const = 0;
    virtual IntersectionVector intersect(Spline* const s) const = 0;
};

#endif //_RENDERABLES_INTERSECTABLE_H_
