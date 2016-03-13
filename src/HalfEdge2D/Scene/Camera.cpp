#include "HalfEdge2D/Scene/Camera.h"

Camera::Camera(Canvas* const canvas) : m_Canvas(canvas), m_2Pi((float)(2.0 * M_PI))
{
    m_Zoom= 1.0f;
    m_Rotation = 0.0f;
    m_Position = QPointF(0.0, 0.0f);
    m_Viewport = QRectF(0.0f, 0.0f, 0.5f, 0.5f);
    m_ViewDirty = true;
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

const Mat3f& Camera::getViewMatrix()
{
    if(m_ViewDirty)
        updateViewMatrix();

    return m_ViewMatrix;
}

const Mat3f& Camera::getInvViewMatrix()
{
    if(m_ViewDirty)
        updateViewMatrix();

    return m_InvViewMatrix;
}

void Camera::setZoom(const float& zoom)
{
    if(zoom == m_Zoom)
        return;

    m_Zoom = zoom;

    m_ViewDirty = true;
}

void Camera::setRotation(const float& rotation)
{
    if(rotation == m_Rotation)
        return;

    m_Rotation = rotation;

    // bring to 0... m_2Pi
    while(m_Rotation < 0.0f)
        m_Rotation += m_2Pi;

    while(m_Rotation >= m_2Pi)
        m_Rotation -= m_2Pi;

    m_ViewDirty = true;
}

void Camera::setPosition(const QPointF& position)
{
    if(position == m_Position)
        return;

    m_Position = position;

    m_ViewDirty = true;
}

void Camera::setViewport(const QRectF& viewport)
{
    m_Viewport = viewport;
}

void Camera::updateViewMatrix()
{
    Mat3f T;
    Mat3f R;
    Mat3f S;

    T <<
        1.0f, 0.0f, -m_Position.x(),
        0.0f, 1.0f, -m_Position.y(),
        0.0f, 0.0f, 1.0f;
    R = Eigen::AngleAxisf(-m_Rotation, Eigen::Vector3f::UnitZ());
    S <<
        m_Zoom, 0.0f, 0.0f,
        0.0f, m_Zoom, 0.0f,
        0.0f, 0.0f, 1.0f;

    m_ViewMatrix = S * T * R;
    m_InvViewMatrix = m_ViewMatrix.inverse();
}
