#ifndef _RENDERABLE_CIRCLE_H_
#define _RENDERABLE_CIRCLE_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"
#include "HalfEdge2D/Renderables/Intersectable.h"

class Circle : public RenderableLine, public Intersectable
{
public:
    Circle();
    Circle(const Circle& other);
    ~Circle();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getRadius() const;
    const Vec2d& getPosition() const;

    //setter
    void setRadius(const float& radius);
    void setPosition(const Vec2d& pos);

    // tooling
    void transform(const Mat3d& mat);

public:
    virtual IntersectionVector intersect(Circle* const c, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Line* const l, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(QuadraticBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(CubicBezier* const b, const bool& valuesFromIntersector = false) const final override;
    virtual IntersectionVector intersect(Spline* const s, const bool& valuesFromIntersector = false) const final override;

private:
    float m_Radius;
    Vec2d m_Position;
};

#endif //_RENDERABLE_CIRCLE_H_
