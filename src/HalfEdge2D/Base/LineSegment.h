#ifndef _BASE_LINESEGMENT_H_
#define _BASE_LINESEGMENT_H_

#include "HalfEdge2D/Base/Vector.h"

class LineSegment
{
public:
    LineSegment();
    ~LineSegment();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const Vec2f& getPositionStart() const;
    const Vec2f& getPositionEnd() const;
    Vec2f getNormal();
    float getLength() const;

    //setter
    void setStart(const Vec2f& pos);
    void setEnd(const Vec2f& pos);

    // tooling
    Vec2f getRelativePoint(const float& pos) const;
    bool collinearTo(const LineSegment& l) const;
    Vec2f intersect(const LineSegment& l, bool* const intersect) const;

private:
    void updateParameter();
    void setNormalFromDirection(const Vec2f& direction);

private:
    Vec2f m_Start;
    Vec2f m_End;
    Vec2f m_Direction;
    Vec2d m_Normal;
    Vec2f m_Min;
    Vec2f m_Max;

    bool m_Valid;

    float m_Lenght;
};

#endif //_BASE_LINESEGMENT_H_
