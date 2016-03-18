#ifndef _MESH_TRIANGLE_H_
#define _MESH_TRIANGLE_H_

#include "HalfEdge2D/Base/Vector.h"

#include <array>

class Triangle : private std::array<size_t, 3>
{
public:
    Triangle();
    ~Triangle();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const size_t& getIdx0() const;
    const size_t& getIdx1() const;
    const size_t& getIdx2() const;
    const Vec4f& getColor() const;

    // setter
    void setIdx(const size_t& idx0, const size_t& idx1, const size_t& idx2);
    void setIdx(const std::array<size_t, 3>& idx);
    void setIdx0(const size_t& idx0);
    void setIdx1(const size_t& idx1);
    void setIdx2(const size_t& idx2);
    void setColor(const Vec4f& color);

private:
    Vec4f m_Color;
};

#endif //_MESH_TRIANGLE_H_
