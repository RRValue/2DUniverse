#include "HalfEdge2D/HalfEdge/HESEdge.h"

HESEdge::HESEdge()
{
    m_From = nullptr;
    m_To = nullptr;
    m_Face = nullptr;
    m_Opposite = nullptr;
    m_Prev = nullptr;
    m_Next = nullptr;

    m_Visited = false;
}

HESEdge::~HESEdge()
{

}

HESVertex* const HESEdge::from() const
{
    return m_From;
}

HESVertex* const HESEdge::to() const
{
    return m_To;
}

HESFace* const HESEdge::face() const
{
    return m_Face;
}

HESEdge* const HESEdge::opposite() const
{
    return m_Opposite;
}

HESEdge* const HESEdge::prev() const
{
    return m_Prev;
}

HESEdge* const HESEdge::next() const
{
    return m_Next;
}

const bool& HESEdge::visited() const
{
    return m_Visited;
}

void HESEdge::setFrom(HESVertex* const vertex)
{
    if(vertex == nullptr)
        return;

    m_From = vertex;
}

void HESEdge::setTo(HESVertex* const vertex)
{
    if(vertex == nullptr)
        return;

    m_To = vertex;
}

void HESEdge::setFace(HESFace* const face)
{
    if(face == nullptr)
        return;

    m_Face = face;
}

void HESEdge::setOpposite(HESEdge* const oppositeEdge)
{
    if(oppositeEdge == nullptr)
        return;

    m_Opposite = oppositeEdge;
}

void HESEdge::setPrev(HESEdge* const prevEdge)
{
    if(prevEdge == nullptr)
        return;

    m_Prev = prevEdge;
}

void HESEdge::setNext(HESEdge* const nextEdge)
{
    if(nextEdge == nullptr)
        return;

    m_Next = nextEdge;
}

void HESEdge::setVisited(const bool& visited)
{
    m_Visited = visited;
}
