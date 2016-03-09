#include "HalfEdge2D/HalfEdge/HESEdge.h"

HESEdge::HESEdge()
{
    m_SourceVertex = nullptr;
    m_IncidentFace = nullptr;
    m_OppositeEdge = nullptr;
    m_PrevEdge = nullptr;
    m_NextEdge = nullptr;
}

HESEdge::~HESEdge()
{

}

HESVertex* const HESEdge::getSourceVertex() const
{
    return m_SourceVertex;
}

HESFace* const HESEdge::getIncidentFace() const
{
    return m_IncidentFace;
}

HESEdge* const HESEdge::getOppositeEdge() const
{
    return m_OppositeEdge;
}

HESEdge* const HESEdge::getPrevEdge() const
{
    return m_PrevEdge;
}

HESEdge* const HESEdge::getNextEdge() const
{
    return m_NextEdge;
}

void HESEdge::setSourceVertex(HESVertex* const vertex)
{
    if(vertex == nullptr)
        return;

    m_SourceVertex = vertex;
}

void HESEdge::setIncidentFace(HESFace* const face)
{
    if(face == nullptr)
        return;

    m_IncidentFace = face;
}

void HESEdge::setOppositeEdge(HESEdge* const oppositeEdge)
{
    if(oppositeEdge == nullptr)
        return;

    m_OppositeEdge = oppositeEdge;
}

void HESEdge::setPrevEdge(HESEdge* const prevEdge)
{
    if(prevEdge == nullptr)
        return;

    m_PrevEdge = prevEdge;
}

void HESEdge::setNextEdge(HESEdge* const nextEdge)
{
    if(nextEdge == nullptr)
        return;

    m_NextEdge = nextEdge;
}
