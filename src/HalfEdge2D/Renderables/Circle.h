#ifndef _RENDERABLE_CIRCLE_H_
#define _RENDERABLE_CIRCLE_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"

class Circle : public RenderableLine
{
public:
    Circle();
    Circle(const Circle& other);
    ~Circle();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getRadius() const;
    const Vec2f& getPosition() const;

    //setter
    void setRadius(const float& radius);
    void setPosition(const Vec2f& pos);

private:
    float m_Radius;
    Vec2f m_Position;
};

#endif //_RENDERABLE_CIRCLE_H_
