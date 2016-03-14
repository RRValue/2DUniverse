#include "HalfEdge2D/HalfEdge2DNavigator.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/RenderTarget.h"

#include <QtGui/QMouseEvent>

HalfEdge2DNavigator::HalfEdge2DNavigator(RenderTarget* const target) : m_RenderTarget(target)
{
    m_Navigatin = false;
    m_ActiveCamera = nullptr;
    m_ZoomFactor = 1.05f;
}

HalfEdge2DNavigator::~HalfEdge2DNavigator()
{

}

bool HalfEdge2DNavigator::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Navigatin)
        return false;

    if(m_ActiveViewPort == nullptr)
        return false;

    m_ActiveCamera = m_ActiveViewPort->getCamera();

    if(m_ActiveCamera == nullptr)
        return false;

    if(event->button() == Qt::RightButton)
        m_Navigatin = true;

    if(!m_Navigatin)
        return false;

    updateTransMatrix();

    QPoint pos_px = event->pos();

    if(!inViewPort(pos_px))
        m_Navigatin = false;

    if(!m_Navigatin)
        return false;

    m_CamMoveInitMousePos = invTrans(pos_px);
    m_CamMoveInitCamPos = m_ActiveCamera->getPosition();

    return true;
}

bool HalfEdge2DNavigator::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_ActiveCamera == nullptr)
        return false;

    if(!m_Navigatin)
        return false;

    m_ActiveCamera->setPosition(m_CamMoveInitCamPos);

    QPoint pos = keepInViewPort(event->pos());
    QPointF current_delta = m_CamMoveInitMousePos - invTrans(pos);

    m_ActiveCamera->setPosition(m_CamMoveInitCamPos + current_delta);

    // update widget
    m_RenderTarget->render();

    return true;
}

bool HalfEdge2DNavigator::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_ActiveCamera == nullptr)
        return false;

    if(!m_Navigatin)
        return false;

    m_Navigatin = false;

    return true;
}

bool HalfEdge2DNavigator::handleResizeEvent(QResizeEvent* const event)
{
    if(m_ActiveCamera == nullptr)
        return false;

    return true;
}

bool HalfEdge2DNavigator::handleWheelEvent(QWheelEvent* const event)
{
    if(m_ActiveViewPort == nullptr)
        return false;

    m_ActiveCamera = m_ActiveViewPort->getCamera();

    if(m_ActiveCamera == nullptr)
        return false;

    updateTransMatrix();

    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    bool controll_pressed = event->modifiers().testFlag(Qt::KeyboardModifier::ControlModifier);

    if(controll_pressed)
    {
        float rot_step = (float)(2.0 * M_PI / (2.0 * 360.0));
        float rot = m_ActiveCamera->getRotation();
        rot += rot_step * (float)(numDegrees.y() / 15);
        m_ActiveCamera->setRotation(rot);

        // update widget
        m_RenderTarget->render();
    }
    else
    {
        if(!numPixels.isNull())
            zoom(numPixels.y(), event->pos());
        else if(!numDegrees.isNull())
        {
            QPoint numSteps = numDegrees / 15;

            zoom(numSteps.y(), event->pos());
        }
    }

    return true;
}

void HalfEdge2DNavigator::zoom(const int& step, const QPoint& pos_px)
{
    if(step == 0)
        return;

    QPointF mouse_pos = invTrans(pos_px);

    // adjust zoom
    float zoom = m_ActiveCamera->getZoom();

    if(step > 0)
        zoom *= float(step) * m_ZoomFactor;
    else
        zoom /= std::abs(float(step)) * m_ZoomFactor;

    m_ActiveCamera->setZoom(zoom);

    // adjust pos (new pos is _zoom_factor / or * delta_pos_scene, cam_pos)
    QPointF d_cam_mouse = m_ActiveCamera->getPosition() - mouse_pos;
    float d_cam_mouse_length = std::sqrt(std::pow(d_cam_mouse.x(), 2.0f) + std::pow(d_cam_mouse.y(), 2.0f));
    float d_cam_mouse_length_new = 1.0f;

    if(step > 0)
        d_cam_mouse_length_new /= float(step) * m_ZoomFactor;
    else
        d_cam_mouse_length_new *= std::abs(float(step)) * m_ZoomFactor;

    QPointF new_pos = QPointF(
        mouse_pos.x() + (d_cam_mouse_length_new * d_cam_mouse.x()),
        mouse_pos.y() + (d_cam_mouse_length_new * d_cam_mouse.y()));

    m_ActiveCamera->setPosition(new_pos);

    // update widget
    m_RenderTarget->render();
}

bool HalfEdge2DNavigator::inViewPort(const QPoint& point) const
{
    Mat3f inv_device_matrix = m_RenderTarget->getInvDeviceMatrix();
    Vec3f dev_coord = inv_device_matrix * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    const QRectF& vp_size = m_ActiveViewPort->getSize();

    if(
        dev_coord(0) >= vp_size.x() && dev_coord(0) < vp_size.x() + vp_size.width() &&
        dev_coord(1) >= vp_size.y() && dev_coord(1) < vp_size.y() + vp_size.height())
        return true;

    return false;
}

QPoint HalfEdge2DNavigator::keepInViewPort(const QPoint& point) const
{
    Mat3f device_matrix = m_RenderTarget->getDeviceMatrix();
    Mat3f inv_device_matrix = m_RenderTarget->getInvDeviceMatrix();
    
    Vec3f dev_coord = inv_device_matrix * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    const QRectF& vp_size = m_ActiveViewPort->getSize();

    if(dev_coord(0) > vp_size.right())
        dev_coord(0) = vp_size.right();
    if(dev_coord(0) < vp_size.left())
        dev_coord(0) = vp_size.left();

    if(dev_coord(1) > vp_size.bottom())
        dev_coord(1) = vp_size.bottom();
    if(dev_coord(1) < vp_size.top())
        dev_coord(1) = vp_size.top();

    dev_coord(2) = 1.0;
    dev_coord = device_matrix * dev_coord;

    return QPoint((int)(dev_coord(0) + 0.5f), (int)(dev_coord(1) + 0.5f));
}

void HalfEdge2DNavigator::updateTransMatrix()
{
    Mat3f V = m_ActiveCamera->getViewMatrix();
    Mat3f P = m_ActiveViewPort->getProjectionMatrix();
    Mat3f D = m_RenderTarget->getDeviceMatrix();

    m_TransMat = D * P * V;
    m_InvTransMat = m_TransMat.inverse();
}

QPointF HalfEdge2DNavigator::trans(const QPointF& point)
{
    Vec3f trans_p = m_TransMat * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    return QPointF(trans_p.x(), trans_p.y());
}

QPointF HalfEdge2DNavigator::invTrans(const QPointF& point)
{
    Vec3f trans_p = m_InvTransMat * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    return QPointF(trans_p.x(), trans_p.y());
}
