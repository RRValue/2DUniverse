#include "HalfEdge2D/HalfEdge2DEventHandler.h"

HalfEdge2DEventHandler::HalfEdge2DEventHandler()
{

}

HalfEdge2DEventHandler::~HalfEdge2DEventHandler()
{

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
