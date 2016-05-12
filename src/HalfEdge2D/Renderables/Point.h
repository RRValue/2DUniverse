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
    const Vec2d& getPosition() const;
    const Vec4f& getColour() const;
    const bool& isVisible() const;

    //setter
    void setSize(const float& size);
    void setPosition(const Vec2d& pos);
    void setColour(const Vec4f& colour);
    void setVisible(const bool& visible);

private:
    float m_Size;
    Vec2d m_Position;
    Vec4f m_Colour;
    bool m_Visible;
};

#endif //_RENDERABLE_POINT_H_
