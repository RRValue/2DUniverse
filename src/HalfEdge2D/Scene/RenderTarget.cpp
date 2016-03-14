#include "HalfEdge2D/Scene/RenderTarget.h"

#include "HalfEdge2D/Scene/ViewPort.h"

#include <cassert>

RenderTarget::RenderTarget()
{
    m_Size = QSizeF(100.0f, 100.0f);
    m_DeviceDirty = true;
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

const Mat3f& RenderTarget::getDeviceMatrix()
{
    if(m_DeviceDirty)
        updateDeviceMatrices();

    return m_DeviceMat;
}

const Mat3f& RenderTarget::getInvDeviceMatrix()
{
    if(m_DeviceDirty)
        updateDeviceMatrices();

    return m_InvDeviceMat;
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

    m_DeviceDirty = true;
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

void RenderTarget::updateDeviceMatrices()
{
    if(!m_DeviceDirty)
        return;

    float t_w = m_Size.width() - 1;
    float t_h = m_Size.height() - 1;

    m_DeviceMat <<
        1.0f * t_w, 0.0f, 0.0f,
        0.0f, -1.0f * t_h, t_h,
        0.0f, 0.0f, 1.0f;

    m_InvDeviceMat = m_DeviceMat.inverse();

    m_DeviceDirty = false;
}
