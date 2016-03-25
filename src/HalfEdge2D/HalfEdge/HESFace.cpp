#include "HalfEdge2D/HalfEdge/HESFace.h"

HESFace::HESFace()
{
    m_Visited = false;
}

HESFace::~HESFace()
{

}

const std::set<HESEdge*>& HESFace::getEdges() const
{
    return m_Edges;
}

const bool& HESFace::visited() const
{
    return m_Visited;
}

void HESFace::setVisited(const bool& visited)
{
    m_Visited = visited;
}

void HESFace::addEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    if(findEdge(edge) != m_Edges.end())
        return;

    m_Edges.insert(edge);
}

void HESFace::removeEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    const std::set<HESEdge*>::const_iterator& find_iter = findEdge(edge);

    if(find_iter == m_Edges.end())
        return;

    m_Edges.erase(find_iter);
}

std::set<HESEdge*>::const_iterator HESFace::findEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return m_Edges.end();

    return m_Edges.find(edge);
}
