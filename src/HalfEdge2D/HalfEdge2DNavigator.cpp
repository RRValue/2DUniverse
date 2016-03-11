#include "HalfEdge2D/HalfEdge2DNavigator.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/Canvas.h"

#include <QtGui/QMouseEvent>

#include <qdebug.h>

HalfEdge2DNavigator::HalfEdge2DNavigator(HalfEdge2DWidget* const widget) : m_Widget(widget)
{
    m_Navigatin = false;
    m_Camera = nullptr;
}

HalfEdge2DNavigator::~HalfEdge2DNavigator()
{

}

void HalfEdge2DNavigator::setCamera(Camera* const camera)
{
    if(camera == nullptr)
        return;

    m_Camera = camera;
}

bool HalfEdge2DNavigator::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Camera == nullptr)
        return false;

    if(m_Navigatin)
        return false;

    if(event->button() == Qt::RightButton)
        m_Navigatin = true;

    if(!m_Navigatin)
        return false;

    QPoint pos_px = event->pos();

    if(!inCanvas(pos_px))
        m_Navigatin = false;

    if(!m_Navigatin)
        return false;

    m_CamMoveInitMousePos = invTransform(pos_px);
    m_CamMoveInitCamPos = m_Camera->getPosition();

    qDebug() << "Start Navigate";

    return true;
}

bool HalfEdge2DNavigator::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_Camera == nullptr)
        return false;

    if(!m_Navigatin)
        return false;

    m_Camera->setPosition(m_CamMoveInitCamPos);

    QPoint pos = keepInCanvas(event->pos());
    QPointF current_delta = m_CamMoveInitMousePos - invTransform(pos);

    m_Camera->setPosition(m_CamMoveInitCamPos + current_delta);

    // update widget
    m_Widget->update();

    qDebug() << "Navigate";

    return true;
}

bool HalfEdge2DNavigator::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Camera == nullptr)
        return false;

    if(!m_Navigatin)
        return false;

    m_Navigatin = false;

    qDebug() << "Stop Navigate";

    return true;
}

bool HalfEdge2DNavigator::handleResizeEvent(QResizeEvent* const event)
{
    if(m_Camera == nullptr)
        return false;

    m_Camera->getCanvas()->setSize(event->size());

    qDebug() << "Resize";

    return true;
}

bool HalfEdge2DNavigator::handleWheelEvent(QWheelEvent* const event)
{
    if(m_Camera == nullptr)
        return false;

    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if(!numPixels.isNull())
        zoom(numPixels.y(), event->pos());
    else if(!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;

        zoom(numSteps.y(), event->pos());
    }

    qDebug() << "Wheel";

    return true;
}

void HalfEdge2DNavigator::zoom(const int& step, const QPoint& pos_px)
{
    if(step == 0)
        return;

    QPointF mouse_pos = invTransform(pos_px);

    // adjust zoom
    QPointF ortho_size = m_Camera->getOrthoSize();
    
    if(step > 0)
        ortho_size *= float(step) * m_ZoomFactor;
    else
        ortho_size /= std::abs(float(step)) * m_ZoomFactor;

    m_Camera->setOrthoSize(ortho_size);

    // adjust pos (new pos is _zoom_factor / or * delta_pos_scene, cam_pos)
    QPointF d_cam_mouse = m_Camera->getPosition() - mouse_pos;
    float d_cam_mouse_length = std::sqrt(std::pow(d_cam_mouse.x(), 2.0f) + std::pow(d_cam_mouse.y(), 2.0f));
    float d_cam_mouse_length_new = 1.0f;

    if(step > 0)
        d_cam_mouse_length_new /= float(step) * m_ZoomFactor;
    else
        d_cam_mouse_length_new *= std::abs(float(step)) * m_ZoomFactor;

    QPointF new_pos = QPointF(
        mouse_pos.x() + (d_cam_mouse_length_new * d_cam_mouse.x()),
        mouse_pos.y() + (d_cam_mouse_length_new * d_cam_mouse.y()));

    m_Camera->setPosition(new_pos);

    // update widget
    m_Widget->update();
}

QPointF HalfEdge2DNavigator::toView(const QPointF& p)
{
    const QPointF& cam_pos = m_Camera->getPosition();
    const QPointF& ortho_size = m_Camera->getPosition();

    return QPointF(
        (p - cam_pos).x() * ortho_size.x(),
        (p - cam_pos).y() * ortho_size.y());
}

QPointF HalfEdge2DNavigator::fromView(const QPointF& p)
{
    const QPointF& cam_pos = m_Camera->getPosition();
    const QPointF& ortho_size = m_Camera->getPosition();

    return QPointF(
        p.x() / ortho_size.x(),
        p.y() / ortho_size.y()) + cam_pos;
}

QPointF HalfEdge2DNavigator::toDeviceCoords(const QPointF& point)
{
    return QPointF(
        ((point.x() * m_Camera->getCanvas()->getAspectRatio()) + 1.0f) / 2.0f,
        (point.y() + 1.0f) / 2.0f);
}

QPointF HalfEdge2DNavigator::fromDeviceCoords(const QPointF& point)
{
    return QPointF(
        ((point.x() * 2.0f) - 1.0f) / m_Camera->getCanvas()->getAspectRatio(),
        (point.y() * 2.0f) - 1.0f);
}

QPointF HalfEdge2DNavigator::toWidgetCoords(const QPointF& p)
{
    const QSize& canvas_size = m_Camera->getCanvas()->getSize();

    return QPointF(
        p.x() * (float)canvas_size.width(),
        (1.0f - p.y()) * (float)canvas_size.height());
}

QPointF HalfEdge2DNavigator::fromWidgetCoords(const QPointF& p)
{
    const QSize& canvas_size = m_Camera->getCanvas()->getSize();

    return QPointF(
        p.x() / (float)canvas_size.width(),
        1.0f - (p.y() / (float)canvas_size.height()));
}

QPointF HalfEdge2DNavigator::transform(const QPointF& point)
{
    return toWidgetCoords(toDeviceCoords(toView(point)));
}

QPointF HalfEdge2DNavigator::invTransform(const QPointF& point)
{
    return fromView(fromDeviceCoords(fromWidgetCoords(point)));
}

bool HalfEdge2DNavigator::inCanvas(const QPoint& point)
{
    const QSize& canvas_size = m_Camera->getCanvas()->getSize();

    if(point.x() < 0 || point.x() >= canvas_size.width())
        return false;

    if(point.y() < 0 || point.y() >= canvas_size.height())
        return false;

    return true;
}

QPoint HalfEdge2DNavigator::keepInCanvas(const QPoint& point)
{
    const QSize& canvas_size = m_Camera->getCanvas()->getSize();
    QPoint pos = point;

    if(pos.x() > canvas_size.width())
        pos.setX(canvas_size.width());
    if(pos.x() < 0)
        pos.setX(0);

    if(pos.y() > canvas_size.height())
        pos.setY(canvas_size.height());
    if(pos.y() < 0)
        pos.setY(0);

    return pos;
}
