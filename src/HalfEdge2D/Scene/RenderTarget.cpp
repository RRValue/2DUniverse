#include "HalfEdge2D/Scene/RenderTarget.h"

#include "HalfEdge2D/Scene/ViewPort.h"

#include <cassert>

RenderTarget::RenderTarget()
{
    m_Size = QSizeF(100.0f, 100.0f);
}

RenderTarget::~RenderTarget()
{

}

const QSizeF& RenderTarget::getSize() const
{
    return m_Size;
}

const std::vector<ViewPort* const>& RenderTarget::getViewPorts() const
{
    return m_ViewPorts;
}

ViewPort* const RenderTarget::getViewPort(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_ViewPorts.size());

    return m_ViewPorts[idx];
}

void RenderTarget::setSize(const QSizeF& size)
{
    if(size == m_Size)
        return;

    m_Size = size;
}

void RenderTarget::setEventHandler(HalfEdge2DEventInterface* const eventInterface)
{
    m_EventInterface = eventInterface;
}

void RenderTarget::addViewPort(ViewPort* const viewport)
{
    if(viewport == nullptr)
        return;

    for(std::vector<ViewPort* const>::iterator iter = m_ViewPorts.begin(); iter != m_ViewPorts.end(); ++iter)
        if(*iter == viewport)
            return;

    m_ViewPorts.push_back(viewport);
}

void RenderTarget::removeViewPort(ViewPort* const viewport)
{
    for(std::vector<ViewPort* const>::iterator iter = m_ViewPorts.begin(); iter != m_ViewPorts.end(); ++iter)
    {
        if(*iter != viewport)
            continue;

        m_ViewPorts.erase(iter);

        break;
    }
}

void RenderTarget::updateViewPortsTargetSize()
{
    for(const auto& vp : m_ViewPorts)
        vp->setTargetSize(m_Size);
}
