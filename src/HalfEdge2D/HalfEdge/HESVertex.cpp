#include "HalfEdge2D/HalfEdge/HESVertex.h"

HESVertex::HESVertex()
{
    m_Visited = false;
}

HESVertex::HESVertex(const HESVertex& other) : Vertex(other), m_Visited(other.m_Visited)
{

}

HESVertex::~HESVertex()
{

}

const std::set<HESEdge*>& HESVertex::getEdges() const
{
    return m_Edges;
}

const bool& HESVertex::visited() const
{
    return m_Visited;
}

void HESVertex::setVisited(const bool& visited)
{
    m_Visited = visited;
}

void HESVertex::addEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    if(findEdge(edge) != m_Edges.end())
        return;

    m_Edges.insert(edge);
}

void HESVertex::removeEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    const std::set<HESEdge*>::const_iterator& find_iter = findEdge(edge);

    if(find_iter == m_Edges.end())
        return;

    m_Edges.erase(find_iter);
}

std::set<HESEdge*>::const_iterator HESVertex::findEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return m_Edges.end();

    return m_Edges.find(edge);
}
