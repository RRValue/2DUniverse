#include "HalfEdge2D/Mesh/Face.h"

Face::Face()
{
    m_Color = Vec4f(0.78f, 0.78f, 0.78f, 1.0f);
}

Face::~Face()
{

}

size_t Face::getNumVert() const
{
    return m_idx.size();
}

const std::vector<size_t>& Face::getVertIds() const
{
    return m_idx;
}

const size_t& Face::getVertIdx(const size_t& idx) const
{
    return m_idx[idx];
}

const Vec4f& Face::getColor() const
{
    return m_Color;
}

void Face::addVertIdx(const size_t& idxVert)
{
    m_idx.push_back(idxVert);
}

void Face::addVertIdx(const std::initializer_list<size_t>& idxs)
{
    for(const auto& idx : idxs)
        addVertIdx(idx);
}

void Face::setVertIdx(const size_t& idx, const size_t& idxVert)
{
    m_idx[idx] = idxVert;
}

void Face::setColor(const Vec4f& color)
{
    m_Color = color;
}
