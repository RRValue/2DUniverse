#ifndef _RENDERABLE_CIRCLE_H_
#define _RENDERABLE_CIRCLE_H_

#include "HalfEdge2D/Base/Vector.h"

class Circle
{
public:
    Circle();
    ~Circle();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getRadius() const;
    const Vec2f& getPosition() const;
    const Vec4f& getColour() const;

    //setter
    void setRadius(const float& radius);
    void setPosition(const Vec2f& pos);
    void setColour(const Vec4f& colour);

private:
    float m_Radius;
    Vec2f m_Position;
    Vec4f m_Colour;
};

#endif //_RENDERABLE_CIRCLE_H_
