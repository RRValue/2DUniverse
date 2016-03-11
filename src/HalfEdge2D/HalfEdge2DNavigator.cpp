#include "HalfEdge2D/HalfEdge2DNavigator.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"

#include <QtGui/QMouseEvent>

HalfEdge2DNavigator::HalfEdge2DNavigator(HalfEdge2DWidget* const widget) : m_Widget(widget)
{
    m_Navigatin = false;
    
    m_OrthoSize = QPointF(1.0f, 1.0f);
    m_CameraPosition = QPointF(0.0, 0.0f);
    m_AspectRatio = 1.0f;
    m_ZoomFactor = 1.05f;
}

HalfEdge2DNavigator::~HalfEdge2DNavigator()
{

}

bool HalfEdge2DNavigator::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Navigatin)
        return false;

    if(event->button() == Qt::RightButton)
        m_Navigatin = true;

    if(!m_Navigatin)
        return false;

    QPoint pos_px = event->pos();

    if(!inWidget(pos_px))
        m_Navigatin = false;

    if(!m_Navigatin)
        return false;

    m_CamMoveInitMousePos = invTransform(pos_px);
    m_CamMoveInitCamPos = m_CameraPosition;

    return true;
}

bool HalfEdge2DNavigator::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(!m_Navigatin)
        return false;

    m_CameraPosition = m_CamMoveInitCamPos;

    QPoint pos = keepInWidget(event->pos());
    QPointF current_delta = m_CamMoveInitMousePos - invTransform(pos);

    m_CameraPosition = m_CamMoveInitCamPos + current_delta;

    // update widget
    m_Widget->update();

    return true;
}

bool HalfEdge2DNavigator::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(!m_Navigatin)
        return false;

    m_Navigatin = false;

    return true;
}

bool HalfEdge2DNavigator::handleResizeEvent(QResizeEvent* const event)
{
    m_CurrentSize = event->size();

    updateRelativeBases();

    return true;
}

bool HalfEdge2DNavigator::handleWheelEvent(QWheelEvent* const event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if(!numPixels.isNull())
        zoom(numPixels.y(), event->pos());
    else if(!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;

        zoom(numSteps.y(), event->pos());
    }

    return true;
}

void HalfEdge2DNavigator::zoom(const int& step, const QPoint& pos_px)
{
    if(step == 0)
        return;

    QPointF mouse_pos = invTransform(pos_px);

    // adjust zoom
    if(step > 0)
        m_OrthoSize *= float(step) * m_ZoomFactor;
    else
        m_OrthoSize /= std::abs(float(step)) * m_ZoomFactor;

    // adjust pos (new pos is _zoom_factor / or * delta_pos_scene, cam_pos)
    QPointF d_cam_mouse = m_CameraPosition - mouse_pos;
    float d_cam_mouse_length = std::sqrt(std::pow(d_cam_mouse.x(), 2.0f) + std::pow(d_cam_mouse.y(), 2.0f));
    float d_cam_mouse_length_new = 1.0f;

    if(step > 0)
        d_cam_mouse_length_new /= float(step) * m_ZoomFactor;
    else
        d_cam_mouse_length_new *= std::abs(float(step)) * m_ZoomFactor;

    m_CameraPosition = QPointF(
        mouse_pos.x() + (d_cam_mouse_length_new * d_cam_mouse.x()),
        mouse_pos.y() + (d_cam_mouse_length_new * d_cam_mouse.y()));

    // update widget
    m_Widget->update();
}

void HalfEdge2DNavigator::updateRelativeBases()
{
    m_AspectRatio = float(m_CurrentSize.height()) / float(m_CurrentSize.width());
}

QPointF HalfEdge2DNavigator::toView(const QPointF& p)
{
    return QPointF(
        (p - m_CameraPosition).x() * m_OrthoSize.x(),
        (p - m_CameraPosition).y() * m_OrthoSize.y());
}

QPointF HalfEdge2DNavigator::fromView(const QPointF& p)
{
    return QPointF(
        p.x() / m_OrthoSize.x(),
        p.y() / m_OrthoSize.y()) + m_CameraPosition;
}

QPointF HalfEdge2DNavigator::toDeviceCoords(const QPointF& point)
{
    return QPointF(
        ((point.x() * m_AspectRatio) + 1.0f) / 2.0f,
        (point.y() + 1.0f) / 2.0f);
}

QPointF HalfEdge2DNavigator::fromDeviceCoords(const QPointF& point)
{
    return QPointF(
        ((point.x() * 2.0f) - 1.0f) / m_AspectRatio,
        (point.y() * 2.0f) - 1.0f);
}

QPointF HalfEdge2DNavigator::toWidgetCoords(const QPointF& p)
{
    return QPointF(
        p.x() * (float)m_CurrentSize.width(),
        (1.0f - p.y()) * (float)m_CurrentSize.height());
}

QPointF HalfEdge2DNavigator::fromWidgetCoords(const QPointF& p)
{
    return QPointF(
        p.x() / (float)m_CurrentSize.width(),
        1.0f - (p.y() / (float)m_CurrentSize.height()));
}

QPointF HalfEdge2DNavigator::transform(const QPointF& point)
{
    return toWidgetCoords(toDeviceCoords(toView(point)));
}

QPointF HalfEdge2DNavigator::invTransform(const QPointF& point)
{
    return fromView(fromDeviceCoords(fromWidgetCoords(point)));
}

bool HalfEdge2DNavigator::inWidget(const QPoint& point)
{
    if(point.x() < 0 || point.x() >= m_CurrentSize.width())
        return false;

    if(point.y() < 0 || point.y() >= m_CurrentSize.height())
        return false;

    return true;
}

QPoint HalfEdge2DNavigator::keepInWidget(const QPoint& point)
{
    QPoint pos = point;

    if(pos.x() > m_CurrentSize.width())
        pos.setX(m_CurrentSize.width());
    if(pos.x() < 0)
        pos.setX(0);

    if(pos.y() > m_CurrentSize.height())
        pos.setY(m_CurrentSize.height());
    if(pos.y() < 0)
        pos.setY(0);

    return pos;
}
