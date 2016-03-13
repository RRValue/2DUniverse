#include "HalfEdge2D/Scene/Camera.h"

Camera::Camera(Canvas* const canvas) : m_Canvas(canvas), m_2Pi((float)(2.0 * M_PI))
{
    m_Zoom= 1.0f;
    m_Rotation = 0.0f;
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

const float& Camera::getRotation() const
{
    return m_Rotation;
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

void Camera::setRotation(const float& rotation)
{
    m_Rotation = rotation;

    // bring to 0... m_2Pi
    while(m_Rotation < 0.0f)
        m_Rotation += m_2Pi;

    while(m_Rotation >= m_2Pi)
        m_Rotation -= m_2Pi;
}

void Camera::setPosition(const QPointF& position)
{
    m_Position = position;
}

void Camera::setViewport(const QRectF& viewport)
{
    m_Viewport = viewport;
}
