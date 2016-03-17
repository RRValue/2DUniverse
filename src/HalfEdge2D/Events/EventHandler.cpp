#include "HalfEdge2D/Events/EventHandler.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/Scene/ViewPort.h"

#include <QtGui/QResizeEvent>

#include <qdebug.h>

EventHandler::EventHandler(QPaintTarget* const paintTarget) : m_PaintTarget(paintTarget)
{
    m_RenderTarget = paintTarget;
    m_Renderer = nullptr;
    m_ActiveViewPort = nullptr;
}

EventHandler::~EventHandler()
{

}

void EventHandler::setNavigator(EventInterface* const navigator)
{
    if(navigator == nullptr)
        return;

    if(navigator == m_Navigator)
        return;

    m_Navigator = navigator;
}

void EventHandler::setController(EventInterface* const controller)
{
    if(controller == nullptr)
        return;

    if(controller == m_Controller)
        return;

    m_Controller = controller;
}

void EventHandler::setRenderer(Renderer* const renderer)
{
    if(renderer == nullptr)
        return;

    m_Renderer = renderer;
    m_Renderer->addPaintTarget(m_PaintTarget);

    if(m_Navigator != nullptr)
        m_Navigator->m_Renderer = m_Renderer;

    if(m_Controller != nullptr)
        m_Controller->m_Renderer = m_Renderer;
}

ViewPort* const EventHandler::getActiveViewPort()
{
    return m_ActiveViewPort;
}

bool EventHandler::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveRenderTarget();
    setActiveViewport(event->pos());
    setActiveCamera();

    if(m_Navigator != nullptr)
        if(m_Navigator->handleMouseMoveEvent(event))
            return true;

    if(m_Controller != nullptr)
        if(m_Controller->handleMouseMoveEvent(event))
            return true;

    return true;
}

bool EventHandler::handleMousePressEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveRenderTarget();
    setActiveViewport(event->pos());
    setActiveCamera();

    if(m_Navigator != nullptr)
        if(m_Navigator->handleMousePressEvent(event))
            return true;

    if(m_Controller != nullptr)
        if(m_Controller->handleMousePressEvent(event))
            return true;

    return true;
}

bool EventHandler::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveRenderTarget();
    setActiveViewport(event->pos());
    setActiveCamera();

    if(m_Navigator != nullptr)
        if(m_Navigator->handleMouseReleaseEvent(event))
            return true;

    if(m_Controller != nullptr)
        if(m_Controller->handleMouseReleaseEvent(event))
            return true;

    return true;
}

bool EventHandler::handleResizeEvent(QResizeEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveRenderTarget();

    m_RenderTarget->setSize(QSizeF(event->size()));

    if(m_Navigator != nullptr)
        if(m_Navigator->handleResizeEvent(event))
            return true;

    if(m_Controller != nullptr)
        if(m_Controller->handleResizeEvent(event))
            return true;

    return true;
}

bool EventHandler::handleWheelEvent(QWheelEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveRenderTarget();
    setActiveViewport(event->pos());
    setActiveCamera();

    if(m_Navigator != nullptr)
        if(m_Navigator->handleWheelEvent(event))
            return true;

    if(m_Controller != nullptr)
        if(m_Controller->handleWheelEvent(event))
            return true;

    return true;
}

void EventHandler::handlePaintEvent(QPaintEvent* const event)
{
    if(m_Renderer == nullptr)
        return;

    setActiveRenderTarget();

    m_RenderTarget->updateViewPortsTargetSize();
    m_Renderer->render(event, m_PaintTarget);
}

void EventHandler::setActiveRenderTarget()
{
    if(m_Navigator != nullptr)
        m_Navigator->m_RenderTarget = m_RenderTarget;

    if(m_Controller != nullptr)
        m_Controller->m_RenderTarget = m_RenderTarget;
}

void EventHandler::setActiveViewport(const QPoint& point)
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

    if(m_Navigator != nullptr)
        m_Navigator->m_ActiveViewPort = m_ActiveViewPort;

    if(m_Controller != nullptr)
        m_Controller->m_ActiveViewPort = m_ActiveViewPort;
}

void EventHandler::setActiveCamera()
{
    m_ActiveCamera = nullptr;

    if(m_ActiveViewPort != nullptr)
        m_ActiveCamera = m_ActiveViewPort->getCamera();

    if(m_Navigator != nullptr)
        m_Navigator->m_ActiveCamera = m_ActiveCamera;

    if(m_Controller != nullptr)
        m_Controller->m_ActiveCamera = m_ActiveCamera;
}
