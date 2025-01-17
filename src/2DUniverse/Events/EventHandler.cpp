#include "2DUniverse/Events/EventHandler.h"

#include "2DUniverse/Controller/Controller.h"

#include "2DUniverse/Rendering/Renderer.h"
#include "2DUniverse/Rendering/RenderTarget.h"
#include "2DUniverse/Rendering/QWidgetTarget.h"

#include "2DUniverse/Scene/ViewPort.h"

#include <QtGui/QResizeEvent>

EventHandler::EventHandler(QWidgetTarget* const widgetTarget) : m_WidgetTarget(widgetTarget)
{
    m_RenderTarget = widgetTarget;
    m_Renderer = nullptr;
    m_ActiveViewPort = nullptr;
    m_ActiveController = nullptr;
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

void EventHandler::addController(Controller* const controller)
{
    if(controller == nullptr)
        return;

    if(m_Controller.find(controller) != m_Controller.end())
        return;

    m_Controller.insert(controller);

    controller->m_RenderTarget = m_RenderTarget;
    controller->m_ActiveCamera = m_ActiveCamera;
    controller->m_ActiveViewPort = m_ActiveViewPort;
    controller->m_Renderer = m_Renderer;
}

void EventHandler::removeController(Controller* const controller)
{
    if(controller == nullptr)
        return;

    const auto& find_iter = m_Controller.find(controller);

    if(find_iter == m_Controller.end())
        return;

    m_Controller.erase(find_iter);
}

void EventHandler::setActiveController(Controller* const controller)
{
    for(const auto& controller : m_Controller)
        controller->m_Active = false;

    if(m_ActiveController != nullptr)
        m_ActiveController->deactivate();

    m_ActiveController = nullptr;

    if(controller == nullptr)
        return;

    const auto& find_iter = m_Controller.find(controller);

    if(find_iter == m_Controller.end())
        return;

    m_ActiveController = *find_iter;
    m_ActiveController->m_Active = true;
    m_ActiveController->m_RenderTarget = m_RenderTarget;
    m_ActiveController->m_ActiveCamera = m_ActiveCamera;
    m_ActiveController->m_ActiveViewPort = m_ActiveViewPort;
    m_ActiveController->m_Renderer = m_Renderer;

    m_ActiveController->activate();

    m_Renderer->render();
}

void EventHandler::setRenderer(Renderer* const renderer)
{
    if(renderer == nullptr)
        return;

    m_Renderer = renderer;
    m_Renderer->addWidgetTarget(m_WidgetTarget);

    if(m_Navigator != nullptr)
        m_Navigator->m_Renderer = m_Renderer;

    for(const auto& controller : m_Controller)
        controller->m_Renderer = m_Renderer;
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
        {
            viewportContentChanged();

            return true;
        }

    if(m_ActiveController != nullptr)
        if(m_ActiveController->handleMouseMoveEvent(event))
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
        {
            viewportContentChanged();

            return true;
        }

    if(m_ActiveController != nullptr)
        if(m_ActiveController->handleMousePressEvent(event))
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
        {
            viewportContentChanged();

            return true;
        }

    if(m_ActiveController != nullptr)
        if(m_ActiveController->handleMouseReleaseEvent(event))
            return true;

    return true;
}

bool EventHandler::handleResizeEvent(QResizeEvent* const event)
{
    if(event == nullptr)
        return false;

    setActiveRenderTarget();

    m_RenderTarget->setSize(QSizeF(event->size()));

    viewportContentChanged();

    if(m_Navigator != nullptr)
        if(m_Navigator->handleResizeEvent(event))
            return true;

    if(m_ActiveController != nullptr)
        if(m_ActiveController->handleResizeEvent(event))
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
        {
            viewportContentChanged();

            return true;
        }

    if(m_ActiveController != nullptr)
        if(m_ActiveController->handleWheelEvent(event))
            return true;

    return true;
}

void EventHandler::handlePaintEvent(QPaintEvent* const event)
{
    if(m_Renderer == nullptr)
        return;

    setActiveRenderTarget();

    m_RenderTarget->updateViewPortsTargetSize();
    m_Renderer->render(event, m_WidgetTarget);
}

void EventHandler::setActiveRenderTarget()
{
    if(m_Navigator != nullptr)
        m_Navigator->m_RenderTarget = m_RenderTarget;

    for(const auto& controller : m_Controller)
        controller->m_RenderTarget = m_RenderTarget;
}

void EventHandler::setActiveViewport(const QPoint& point)
{
    Mat3f inv_device_matrix = m_RenderTarget->getInvDeviceMatrix();
    Vec3f dev_coord = inv_device_matrix * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    ViewPort* new_viewport = nullptr;

    for(const auto& vp : m_RenderTarget->getViewPorts())
    {
        const QRectF& vp_size = vp->getSize();

        if(
            dev_coord(0) >= vp_size.left() && dev_coord(0) < vp_size.right() &&
            dev_coord(1) >= vp_size.top() && dev_coord(1) < vp_size.bottom())
        {
            new_viewport = vp;

            break;
        }
    }

    if(new_viewport == m_ActiveViewPort)
        return;

    m_ActiveViewPort = new_viewport;

    if(m_Navigator != nullptr)
        m_Navigator->m_ActiveViewPort = m_ActiveViewPort;

    for(const auto& controller : m_Controller)
    {
        controller->m_ActiveViewPort = m_ActiveViewPort;
        controller->m_ViewportContentChanges = true;
    }
}

void EventHandler::setActiveCamera()
{
    Camera* new_camera = nullptr;

    if(m_ActiveViewPort != nullptr)
        new_camera = m_ActiveViewPort->getCamera();

    if(new_camera == m_ActiveCamera)
        return;

    m_ActiveCamera = new_camera;

    if(m_Navigator != nullptr)
        m_Navigator->m_ActiveCamera = m_ActiveCamera;

    for(const auto& controller : m_Controller)
    {
        controller->m_ActiveCamera = m_ActiveCamera;
        controller->m_ViewportContentChanges = true;
    }
}

void EventHandler::viewportContentChanged()
{
    for(const auto& contoller : m_Controller)
        contoller->m_ViewportContentChanges = true;
}
