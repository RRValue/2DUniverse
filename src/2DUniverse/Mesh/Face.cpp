#include "2DUniverse/Mesh/Face.h"

Face::Face()
{
    m_Colour = Vec4f(0.78f, 0.78f, 0.78f, 1.0f);
}

Face::Face(const Face& other)
{
    m_Colour = other.m_Colour;
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

const Vec4f& Face::getColour() const
{
    return m_Colour;
}

void Face::addVertIdx(const size_t& idxVert)
{
    m_idx.push_back(idxVert);
}

void Face::addVertIdx(const std::vector<size_t>& idxs)
{
    for(const auto& idx : idxs)
        addVertIdx(idx);
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

void Face::setVertIdx(const std::vector<size_t>& idxs)
{
    m_idx = idxs;
}

void Face::setColour(const Vec4f& color)
{
    m_Colour = color;
}
