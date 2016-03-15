#include "HalfEdge2D/Scene/ViewPort.h"

#include "HalfEdge2D/Scene/Camera.h"

ViewPort::ViewPort()
{
    m_Size = QRectF(0.0f, 0.0f, 1.0f, 1.0f);
    m_Camera = nullptr;
    m_ProjectionDirty = true;
}

ViewPort::~ViewPort()
{

}

const QRectF& ViewPort::getSize() const
{
    return m_Size;
}

const QSizeF& ViewPort::getTargetSize() const
{
    return m_TargetSize;
}

Camera* const ViewPort::getCamera() const
{
    return m_Camera;
}

const Mat3f& ViewPort::getProjectionMatrix()
{
    if(m_ProjectionDirty)
        updateProjectionMatrix();

    return m_ProjectionMatrix;
}

const Mat3f& ViewPort::getInvProjectionMatrix()
{
    if(m_ProjectionDirty)
        updateProjectionMatrix();

    return m_InvProjectionMatrix;
}

void ViewPort::setSize(const QRectF& size)
{
    if(size == m_Size)
        return;

    m_Size = size;

    m_ProjectionDirty = true;
}

void ViewPort::setTargetSize(const QSizeF& targetSize)
{
    if(targetSize == m_TargetSize)
        return;

    m_TargetSize = targetSize;

    m_ProjectionDirty = true;
}

void ViewPort::setCamera(Camera* const camera)
{
    m_Camera = camera;
}

void ViewPort::updateProjectionMatrix()
{
    float ar = (m_TargetSize.height() * m_Size.height()) / (m_TargetSize.width() * m_Size.width());

    float vpw_half = m_Size.width() / 2.0f;
    float vph_half = m_Size.height() / 2.0f;

    float A = vpw_half * ar;
    float B = vpw_half + m_Size.x();
    float C = vph_half;
    float D = vph_half + m_Size.y();

    m_ProjectionMatrix <<
        A, 0.0f, B,
        0.0f, C, D,
        0.0f, 0.0f, 1.0f;

    m_InvProjectionMatrix = m_ProjectionMatrix.inverse();

    m_ProjectionDirty = false;
}
