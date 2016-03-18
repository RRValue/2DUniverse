#include "HalfEdge2D/Rendering/QWidgetTarget.h"

#include "HalfEdge2D/EVents/EventInterface.h"

QWidgetTarget::QWidgetTarget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    setMouseTracking(true);
}

QWidgetTarget::~QWidgetTarget()
{

}

void QWidgetTarget::mouseMoveEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseMoveEvent(event);
}

void QWidgetTarget::mousePressEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMousePressEvent(event);
}

void QWidgetTarget::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseReleaseEvent(event);
}

void QWidgetTarget::resizeEvent(QResizeEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleResizeEvent(event);
}

void QWidgetTarget::wheelEvent(QWheelEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleWheelEvent(event);
}

void QWidgetTarget::paintEvent(QPaintEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handlePaintEvent(event);
}

void QWidgetTarget::render()
{
    update();
}
