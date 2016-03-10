#include "HalfEdge2D/HalfEdge/HESVertex.h"

HESVertex::HESVertex()
{

}

HESVertex::~HESVertex()
{

}

const std::set<HESEdge*>& HESVertex::getIncidentEdges() const
{
    return m_IncidentEdges;
}

void HESVertex::addIncidentEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    if(findIncidentEdge(edge) != m_IncidentEdges.end())
        return;

    m_IncidentEdges.insert(edge);
}

void HESVertex::removeIncidentEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return;

    const std::set<HESEdge*>::const_iterator& find_iter = findIncidentEdge(edge);

    if(find_iter == m_IncidentEdges.end())
        return;

    m_IncidentEdges.erase(find_iter);
}

std::set<HESEdge*>::const_iterator HESVertex::findIncidentEdge(HESEdge* const edge)
{
    if(edge == nullptr)
        return m_IncidentEdges.end();

    return m_IncidentEdges.find(edge);
}
