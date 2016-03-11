#include "HalfEdge2D/HalfEdge2DNavigator.h"

HalfEdge2DNavigator::HalfEdge2DNavigator(HalfEdge2DWidget* const widget) : m_Widget(widget)
{

}

HalfEdge2DNavigator::~HalfEdge2DNavigator()
{

}

bool HalfEdge2DNavigator::handleMousePressEvent(QMouseEvent* const event)
{
    return true;
}

bool HalfEdge2DNavigator::handleMouseMoveEvent(QMouseEvent* const event)
{
    return true;
}

bool HalfEdge2DNavigator::handleMouseReleaseEvent(QMouseEvent* const event)
{
    return true;
}

bool HalfEdge2DNavigator::handleResizeEvent(QResizeEvent* const event)
{
    return true;
}

bool HalfEdge2DNavigator::handleWheelEvent(QWheelEvent* const event)
{
    return true;
}
