#include "HalfEdge2D/Renderables/CubicBezier.h"

CubicBezier::CubicBezier()
{
    m_Thickness = 0.01f;
}

CubicBezier::CubicBezier(const CubicBezier& other) : CBez2F(other), Renderable(other), m_Thickness(other.m_Thickness)
{

}

CubicBezier::~CubicBezier()
{

}

const float& CubicBezier::getThickness() const
{
    return m_Thickness;
}

void CubicBezier::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}
