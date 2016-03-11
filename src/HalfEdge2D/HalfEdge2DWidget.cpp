#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

HalfEdge2DWidget::HalfEdge2DWidget(QWidget* parent, Qt::WindowFlags f) : 
    QWidget(parent, f), 
    m_EventInterface(nullptr)
{

}

HalfEdge2DWidget::~HalfEdge2DWidget()
{

}

void HalfEdge2DWidget::setEventHandler(HalfEdge2DEventInterface* const eventInterface)
{
    m_EventInterface = eventInterface;
}

void HalfEdge2DWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseMoveEvent(event);
}

void HalfEdge2DWidget::mousePressEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMousePressEvent(event);
}

void HalfEdge2DWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseReleaseEvent(event);
}

void HalfEdge2DWidget::resizeEvent(QResizeEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleResizeEvent(event);
}

void HalfEdge2DWidget::wheelEvent(QWheelEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleWheelEvent(event);
}

void HalfEdge2DWidget::paintEvent(QPaintEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handlePaintEvent(event);
}
