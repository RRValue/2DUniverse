#include "HalfEdge2D/Renderables/RenderableLine.h"

RenderableLine::RenderableLine()
{
    m_Thickness = 0.01f;
}

RenderableLine::RenderableLine(const RenderableLine& other) : Renderable(other), m_Thickness(other.m_Thickness)
{

}

RenderableLine::RenderableLine(RenderableLine&& other) : Renderable(std::move(other)), m_Thickness(std::move(other.m_Thickness))
{

}

RenderableLine::~RenderableLine()
{

}

const float& RenderableLine::getThickness() const
{
    return m_Thickness;
}

void RenderableLine::setThickness(const float& thickness)
{
    m_Thickness = thickness;
}
