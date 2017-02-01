#include "2DUniverse/Rendering/QWidgetTarget.h"

#include "2DUniverse/Rendering/RenderWidget.h"

#include "2DUniverse/EVents/EventInterface.h"

#include <QtGui/QMouseEvent>

QWidgetTarget::QWidgetTarget(RenderWidget* renderWidget) : m_RenderWidget(renderWidget)
{
    connect(m_RenderWidget, &RenderWidget::onMouseMoveEvent, this, &QWidgetTarget::handleMouseMoveEvent);
    connect(m_RenderWidget, &RenderWidget::onMousePressEvent, this, &QWidgetTarget::handleMousePressEvent);
    connect(m_RenderWidget, &RenderWidget::onMouseReleaseEvent, this, &QWidgetTarget::handleMouseReleaseEvent);
    connect(m_RenderWidget, &RenderWidget::onResizeEvent, this, &QWidgetTarget::handleResizeEvent);
    connect(m_RenderWidget, &RenderWidget::onPaintEvent, this, &QWidgetTarget::handlePaintEvent);
    connect(m_RenderWidget, &RenderWidget::onWheelEvent, this, &QWidgetTarget::handleWheelEvent);
}

QWidgetTarget::~QWidgetTarget()
{

}

QPaintDevice* const QWidgetTarget::getPaintDevice() const
{
    return m_RenderWidget;
}

void QWidgetTarget::handleMouseMoveEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseMoveEvent(event);
}

void QWidgetTarget::handleMousePressEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMousePressEvent(event);
}

void QWidgetTarget::handleMouseReleaseEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseReleaseEvent(event);
}

void QWidgetTarget::handleResizeEvent(QResizeEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleResizeEvent(event);
}

void QWidgetTarget::handleWheelEvent(QWheelEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleWheelEvent(event);
}

void QWidgetTarget::handlePaintEvent(QPaintEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handlePaintEvent(event);
}

void QWidgetTarget::render()
{
    m_RenderWidget->update();
}
