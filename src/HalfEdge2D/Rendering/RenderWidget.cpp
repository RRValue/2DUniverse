#include "HalfEdge2D/Rendering/RenderWidget.h"

RenderWidget::RenderWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    setMouseTracking(true);
}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    emit onMouseMoveEvent(event);
}

void RenderWidget::mousePressEvent(QMouseEvent* event)
{
    emit onMousePressEvent(event);
}

void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    emit onMouseReleaseEvent(event);
}

void RenderWidget::resizeEvent(QResizeEvent* event)
{
    emit onResizeEvent(event);
}

void RenderWidget::wheelEvent(QWheelEvent* event)
{
    emit onWheelEvent(event);
}

void RenderWidget::paintEvent(QPaintEvent* event)
{
    emit onPaintEvent(event);
}
