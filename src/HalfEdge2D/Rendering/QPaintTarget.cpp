#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/EVents/EventInterface.h"

QPaintTarget::QPaintTarget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{

}

QPaintTarget::~QPaintTarget()
{

}

void QPaintTarget::mouseMoveEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseMoveEvent(event);
}

void QPaintTarget::mousePressEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMousePressEvent(event);
}

void QPaintTarget::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseReleaseEvent(event);
}

void QPaintTarget::resizeEvent(QResizeEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleResizeEvent(event);
}

void QPaintTarget::wheelEvent(QWheelEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleWheelEvent(event);
}

void QPaintTarget::paintEvent(QPaintEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handlePaintEvent(event);
}

void QPaintTarget::render()
{
    update();
}
