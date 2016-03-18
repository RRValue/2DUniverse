#include "HalfEdge2D/Mesh/Triangle.h"

Triangle::Triangle()
{
    at(0) = 0;
    at(1) = 0;
    at(2) = 0;
    m_Color = Vec4f(0.78f, 0.78f, 0.78f, 1.0f);
}

Triangle::~Triangle()
{

}

const size_t& Triangle::getIdx0() const
{
    return at(0);
}

const size_t& Triangle::getIdx1() const
{
    return at(1);
}

const size_t& Triangle::getIdx2() const
{
    return at(2);
}

const Vec4f& Triangle::getColor() const
{
    return m_Color;
}

void Triangle::setIdx(const size_t& idx0, const size_t& idx1, const size_t& idx2)
{
    at(0) = idx0;
    at(1) = idx1;
    at(2) = idx2;
}

void Triangle::setIdx(const std::array<size_t, 3>& idx)
{
    at(0) = idx[0];
    at(1) = idx[1];
    at(2) = idx[2];
}

void Triangle::setIdx0(const size_t& idx0)
{
    at(0) = idx0;
}

void Triangle::setIdx1(const size_t& idx1)
{
    at(1) = idx1;
}

void Triangle::setIdx2(const size_t& idx2)
{
    at(2) = idx2;
}

void Triangle::setColor(const Vec4f& color)
{
    m_Color = color;
}
