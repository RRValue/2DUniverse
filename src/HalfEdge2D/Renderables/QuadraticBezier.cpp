#include "HalfEdge2D/Renderables/QuadraticBezier.h"

QuadraticBezier::QuadraticBezier()
{
    m_Thickness = 0.01f;
}

QuadraticBezier::QuadraticBezier(const QuadraticBezier& other) : QBez2F(other), Renderable(other), m_Thickness(other.m_Thickness)
{

}

QuadraticBezier::~QuadraticBezier()
{

}

const float& QuadraticBezier::getThickness() const
{
    return m_Thickness;
}

void QuadraticBezier::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}
