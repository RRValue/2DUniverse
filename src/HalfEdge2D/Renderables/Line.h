#ifndef _RENDERABLE_LINE_H_
#define _RENDERABLE_LINE_H_

#include "HalfEdge2D/Base/Vector.h"
#include "HalfEdge2D/Base/StaticNGradeBlend.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"

class Line : protected StaticNGradeBlend<Vec2f, 2>, protected StaticPolynomialSolver<float, 1>
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
    const bool& isVisible() const;

    //setter
    void setThickness(const float& thickness);
    void setPositionStart(const Vec2f& pos);
    void setPositionEnd(const Vec2f& pos);
    void setColour(const Vec4f& colour);
    void setVisible(const bool& visible);

    // tooling
    Vec2f getRelativePoint(const float& pos) const;
    bool collinearTo(const Line& l) const;
    Vec2f intersect(const Line& l, bool* const intersect) const;
    float getLength() const;
    Vec2f getNormal() const;

private:
    float m_Thickness;
    Vec2f m_Start;
    Vec2f m_End;
    Vec4f m_Colour;
    bool m_Visible;
};

#endif //_RENDERABLE_LINE_H_
