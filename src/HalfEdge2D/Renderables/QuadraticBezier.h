#ifndef _RENDERABLE_QUDRATICBEZIER_H_
#define _RENDERABLE_QUDRATICBEZIER_H_

#include "HalfEdge2D/Base/Vector.h"
#include "HalfEdge2D/Base/StaticNGradeBlend.h"

#include <array>

class QuadraticBezier : protected StaticNGradeBlend<Vec2f, 3>
{
public:
    QuadraticBezier();
    ~QuadraticBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const float& getThickness() const;
    const std::array<Vec2f, 3>& getPoints() const;
    const Vec2f& getPoint(const size_t& idx) const;
    const Vec4f& getColour() const;
    const bool& isVisible() const;
    Vec2f pointAt(const float& alpha);

    //setter
    void setThickness(const float& thickness);
    void setPoint(const size_t& idx, const Vec2f& point);
    void setColour(const Vec4f& colour);
    void setVisible(const bool& visible);

private:
    float m_Thickness;
    Vec4f m_Colour;
    bool m_Visible;

    std::array<Vec2f, 3> m_Points;
};

#endif //_RENDERABLE_QUDRATICBEZIER_H_
