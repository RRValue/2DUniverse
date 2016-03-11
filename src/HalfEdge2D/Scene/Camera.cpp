#include "HalfEdge2D/Scene/Camera.h"

Camera::Camera(Canvas* const canvas) : m_Canvas(canvas)
{
    m_OrthoSize = QPointF(1.0f, 1.0f);
    m_Position = QPointF(0.0, 0.0f);
}

Camera::~Camera()
{

}

const QPointF& Camera::getOrthoSize() const
{
    return m_OrthoSize;
}

const QPointF& Camera::getPosition() const
{
    return m_Position;
}

Canvas* const Camera::getCanvas() const
{
    return m_Canvas;
}

void Camera::setOrthoSize(const QPointF& orthoSize)
{
    m_OrthoSize = orthoSize;
}

void Camera::setPosition(const QPointF& position)
{
    m_Position = position;
}
