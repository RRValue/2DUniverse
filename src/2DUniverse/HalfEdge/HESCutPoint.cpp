#include "2DUniverse/HalfEdge/HESCutPoint.h"

#include "2DUniverse/HalfEdge/HESEdge.h"
#include "2DUniverse/HalfEdge/HESVertex.h"

HESCutPoint::HESCutPoint(HESEdge* const e, const Vec2d& p, const bool& border, const double& alpha)
{
    m_Edge = e;
    m_Point = p;
    m_IsOnBorder = border;
    m_Alpha = alpha;
    m_IsOnVertex = false;
    m_Vertex = nullptr;
}

HESCutPoint::HESCutPoint(const HESCutPoint& other)
{
    m_Edge = other.m_Edge;
    m_Point = other.m_Point;
    m_Vertex = other.m_Vertex;
    m_IsOnVertex = other.m_IsOnVertex;
    m_IsOnBorder = other.m_IsOnBorder;
    m_Alpha = other.m_Alpha;
}

void HESCutPoint::snapToVertex()
{
    HESVertex* const v0 = m_Edge->from();
    HESVertex* const v1 = m_Edge->to();

    Vec2d p0 = v0->getPosition();
    Vec2d p1 = v1->getPosition();

    double d0 = (p0 - m_Point).norm();
    double d1 = (p1 - m_Point).norm();

    if(d0 > m_VertexDistanceThreshold && d1 > m_VertexDistanceThreshold)
        m_IsOnVertex = false;
    else
        m_IsOnVertex = true;

    if(d0 < d1)
        m_Vertex = v0;
    else
        m_Vertex = v1;
}

HESCutPoint& HESCutPoint::operator=(HESCutPoint& other)
{
    m_Edge = other.m_Edge;
    m_Point = other.m_Point;
    m_Vertex = other.m_Vertex;
    m_IsOnVertex = other.m_IsOnVertex;
    m_IsOnBorder = other.m_IsOnBorder;
    m_Alpha = other.m_Alpha;

    return *this;
}

bool HESCutPoint::hasSameVertex(const HESCutPoint& cutPoint) const
{
    return m_IsOnVertex && cutPoint.m_IsOnVertex && m_Vertex == cutPoint.m_Vertex;
}

bool HESCutPoint::cutsTriangle(const HESCutPoint& cutPoint) const
{
    return m_Edge != cutPoint.m_Edge && m_Edge->face() == cutPoint.m_Edge->face();
}
