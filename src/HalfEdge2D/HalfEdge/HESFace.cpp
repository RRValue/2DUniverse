#include "HalfEdge2D/HalfEdge/HESFace.h"

HESFace::HESFace()
{

}

HESFace::~HESFace()
{

}

const std::set<HESEdge*>& HESFace::getBoundingEdges() const
{
    return m_BoundingEdges;
}

void HESFace::addBoundingEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    if(findBoundingEdge(edge) != m_BoundingEdges.end())
        return;

    m_BoundingEdges.insert(edge);
}

void HESFace::removeBoundingEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    const std::set<HESEdge*>::const_iterator& find_iter = findBoundingEdge(edge);

    if(find_iter == m_BoundingEdges.end())
        return;

    m_BoundingEdges.erase(find_iter);
}

std::set<HESEdge*>::const_iterator HESFace::findBoundingEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return m_BoundingEdges.end();

    return m_BoundingEdges.find(edge);
}
