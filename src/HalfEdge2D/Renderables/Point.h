#ifndef _RENDERABLE_POINT_H_
#define _RENDERABLE_POINT_H_

#include "HalfEdge2D/Base/Vector.h"

class Point
{
public:
    Point();
    ~Point();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getSize() const;
    const Vec2f& getPosition() const;
    const Vec4f& getColour() const;

    //setter
    void setSize(const float& size);
    void setPosition(const Vec2f& pos);
    void setColour(const Vec4f& colour);

private:
    float m_Size;
    Vec2f m_Position;
    Vec4f m_Colour;
};

#endif //_RENDERABLE_POINT_H_
