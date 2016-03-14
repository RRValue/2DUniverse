#include "HalfEdge2D/HalfEdge2DEventHandler.h"

#include "HalfEdge2D/HalfEdge2DRenderer.h"
#include "HalfEdge2D/Scene/RenderTarget.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include <QtGui/QResizeEvent>

#include <qdebug.h>

HalfEdge2DEventHandler::HalfEdge2DEventHandler(RenderTarget* const renderTarget) : m_RenderTarget(renderTarget)
{
    m_Renderer = nullptr;
    m_ActiveViewPort = nullptr;
}

HalfEdge2DEventHandler::~HalfEdge2DEventHandler()
{

}

void HalfEdge2DEventHandler::setRenderer(HalfEdge2DRenderer* const renderer)
{
    if(renderer == nullptr)
        return;

    m_Renderer = renderer;
}

void HalfEdge2DEventHandler::addEventInterface(HalfEdge2DEventInterface* const eventInterface)
{
    if(eventInterface == nullptr)
        return;

    for(const auto& iface : m_EventInterfaces)
        if(iface == eventInterface)
            return;

    m_EventInterfaces.push_back(eventInterface);
}

ViewPort* const HalfEdge2DEventHandler::getActiveViewPort()
{
    return m_ActiveViewPort;
}

bool HalfEdge2DEventHandler::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveViewport(event->pos());

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleMouseMoveEvent(event))
            return true;

    return true;
}

bool HalfEdge2DEventHandler::handleMousePressEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveViewport(event->pos());

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleMousePressEvent(event))
            return true;

    return true;
}

bool HalfEdge2DEventHandler::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveViewport(event->pos());

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleMouseReleaseEvent(event))
            return true;

    return true;
}

bool HalfEdge2DEventHandler::handleResizeEvent(QResizeEvent* const event)
{
    if(event == nullptr)
        return false;

    m_RenderTarget->setSize(QSizeF(event->size()));

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleResizeEvent(event))
            return true;

    return true;
}

bool HalfEdge2DEventHandler::handleWheelEvent(QWheelEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveViewport(event->pos());

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleWheelEvent(event))
            return true;

    return true;
}

void HalfEdge2DEventHandler::handlePaintEvent(QPaintEvent* const event)
{
    if(m_Renderer == nullptr)
        return;

    m_RenderTarget->updateViewPortsTargetSize();
    m_Renderer->render(event, m_RenderTarget);
}

void HalfEdge2DEventHandler::setActiveViewport(const QPoint& point)
{
    Mat3f inv_device_matrix = m_RenderTarget->getInvDeviceMatrix();
    Vec3f dev_coord = inv_device_matrix * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    m_ActiveViewPort = nullptr;

    for(const auto& vp : m_RenderTarget->getViewPorts())
    {
        const QRectF& vp_size = vp->getSize();

        if(
            dev_coord(0) >= vp_size.left() && dev_coord(0) < vp_size.right() &&
            dev_coord(1) >= vp_size.top() && dev_coord(1) < vp_size.bottom())
        {
            m_ActiveViewPort = vp;

            break;
        }
    }

    for(const auto& idface : m_EventInterfaces)
        idface->m_ActiveViewPort = m_ActiveViewPort;
}
