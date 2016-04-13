#ifndef _RENDERABLE_CUBICBEZIER_H_
#define _RENDERABLE_CUBICBEZIER_H_

#include "HalfEdge2D/Base/Vector.h"
#include "HalfEdge2D/Base/StaticNGradeBlend.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"

#include <array>

class CubicBezier : protected StaticNGradeBlend<Vec2f, 4>, protected StaticPolynomialSolver<float, 2>
{
public:
    CubicBezier();
    ~CubicBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const float& getThickness() const;
    const std::array<Vec2f, 4>& getPoints() const;
    const Vec2f& getPoint(const size_t& idx) const;
    const Vec4f& getColour() const;
    const bool& isVisible() const;
    Vec2f pointAt(const float& alpha) const;

    //setter
    void setThickness(const float& thickness);
    void setPoint(const size_t& idx, const Vec2f& point);
    void setColour(const Vec4f& colour);
    void setVisible(const bool& visible);

    // tooling
    void transform(const Mat3f& m);
    CubicBezier transformed(const Mat3f& m) const;
    std::vector<float> rootsX() const;
    std::vector<float> rootsY() const;

private:
    float m_Thickness;
    Vec4f m_Colour;
    bool m_Visible;

    std::array<Vec2f, 4> m_Points;
};

#endif //_RENDERABLE_CUBICBEZIER_H_
