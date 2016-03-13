#include "HalfEdge2D/Scene/Camera.h"

Camera::Camera(Canvas* const canvas) : m_Canvas(canvas)
{
    m_Zoom= 1.0f;
    m_Position = QPointF(0.0, 0.0f);
    m_Viewport = QRectF(0.0f, 0.0f, 0.5f, 0.5f);
}

Camera::~Camera()
{

}

const float& Camera::getZoom() const
{
    return m_Zoom;
}

const QPointF& Camera::getPosition() const
{
    return m_Position;
}

const QRectF& Camera::getViewport() const
{ 
    return m_Viewport;
}

Canvas* const Camera::getCanvas() const
{
    return m_Canvas;
}

void Camera::setZoom(const float& zoom)
{
    m_Zoom = zoom;
}

void Camera::setPosition(const QPointF& position)
{
    m_Position = position;
}

void Camera::setViewport(const QRectF& viewport)
{
    m_Viewport = viewport;
}
