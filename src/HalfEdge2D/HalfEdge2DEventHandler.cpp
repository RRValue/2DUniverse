#include "HalfEdge2D/HalfEdge2DEventHandler.h"

#include "HalfEdge2D/HalfEdge2DRenderer.h"
#include "HalfEdge2D/Scene/RenderTarget.h"

#include <QtGui/QResizeEvent>

HalfEdge2DEventHandler::HalfEdge2DEventHandler(RenderTarget* const renderTarget) : m_RenderTarget(renderTarget)
{
    m_Renderer = nullptr;
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

bool HalfEdge2DEventHandler::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleMouseMoveEvent(event))
            return true;

    return true;
}

bool HalfEdge2DEventHandler::handleMousePressEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

    for(const auto& idface : m_EventInterfaces)
        if(idface->handleMousePressEvent(event))
            return true;

    return true;
}

bool HalfEdge2DEventHandler::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(event == nullptr)
        return false;

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
