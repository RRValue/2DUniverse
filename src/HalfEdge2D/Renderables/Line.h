#ifndef _RENDERABLE_LINE_H_
#define _RENDERABLE_LINE_H_

#include "HalfEdge2D/Base/Vector.h"

class Line
{
public:
    Line();
    ~Line();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getThickness() const;
    const Vec2f& getPositionStart() const;
    const Vec2f& getPositionEnd() const;
    const Vec4f& getColour() const;

    //setter
    void setThickness(const float& thickness);
    void setPositionStart(const Vec2f& pos);
    void setPositionEnd(const Vec2f& pos);
    void setColour(const Vec4f& colour);

private:
    float m_Thickness;
    Vec2f m_PositionStart;
    Vec2f m_PositionEnd;
    Vec4f m_Colour;
};

#endif //_RENDERABLE_LINE_H_
