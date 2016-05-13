#include "HalfEdge2D/HalfEdge/HESCut.h"

#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESVertex.h"

HESCutPoint::HESCutPoint(HESEdge* const e, const Vec2d& p)
{
    m_Edge = e;
    m_Point = p;

    checkOnVertex();
}

void HESCutPoint::checkOnVertex()
{
    HESVertex* const v0 = m_Edge->from();
    HESVertex* const v1 = m_Edge->to();

    Vec2d p0 = v0->getPosition();
    Vec2d p1 = v1->getPosition();

    double d0 = (p0 - m_Point).norm();
    double d1 = (p0 - m_Point).norm();

    if(d0 > m_VertexDistanceThreshold || d1 > m_VertexDistanceThreshold)
    {
        m_IsOnVertex = false;
        m_NearestVertex = nullptr;

        return;
    }

    m_IsOnVertex = true;

    if(d0 < d1)
        m_NearestVertex = v0;
    else
        m_NearestVertex = v1;
}
