#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

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
    if(m_EventInterface == nullptr)
        return;

    m_EventInterface->handleMouseReleaseEvent(event);
}

void HalfEdge2DWidget::resizeEvent(QResizeEvent* event)
{
    if(m_EventInterface == nullptr)
        return;

    m_EventInterface->handleResizeEvent(event);
}

void HalfEdge2DWidget::paintEvent(QPaintEvent* event)
{
    /*QPainter painter(this);

    for(const auto& p : _points)
        painter.drawEllipse(transform(p), _point_size_px, _point_size_px);

    painter.drawLine(transform(QPointF(-1.0f, 0.0f)), transform(QPointF(1.0f, 0.0f)));
    painter.drawLine(transform(QPointF(0.0f, -1.0f)), transform(QPointF(0.0f, 1.0f)));*/
}
