#include "HalfEdge2D/Controlling/Controller.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include <QtGui/QMouseEvent>

Controller::Controller()
{
    m_MovePoint = false;

    m_ActiveCamera = nullptr;
}

Controller::~Controller()
{

}

void Controller::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    if(scene == m_Scene)
        return;

    m_Scene = scene;
}

bool Controller::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    QPointF pos = keepInViewPort(event->pos()) + m_CurrentHitDistance;
    m_Scene->setPointPos(m_CurrentIdx, invTrans(pos));

    m_Renderer->render();
    
    return true;
}

bool Controller::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(m_MovePoint)
        return false;

    if(m_ActiveViewPort == nullptr)
        return false;

    m_ActiveCamera = m_ActiveViewPort->getCamera();

    if(m_ActiveCamera == nullptr)
        return false;

    updateTransMatrix();

    // set to move mode
    if(event->button() == Qt::LeftButton)
        m_MovePoint = true;
    
    if(!m_MovePoint)
        return false;

    QPointF p = keepInViewPort(event->pos());

    int result = m_Scene->getPointAtPos(invTrans(p));

    // if hit nothing and point size < 4 -> add
    if(result == -1)
    {
        m_Scene->addPoint(invTrans(p));

        m_MovePoint = false;

        m_Renderer->render();
    }
    else
    {
        m_CurrentIdx = result;
        m_CurrentHitDistance = trans(m_Scene->getPoint(m_CurrentIdx)) - p;
    }

    return true;
}

bool Controller::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(event == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    m_MovePoint = false;

    return true;
}

bool Controller::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool Controller::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

bool Controller::inViewPort(const QPoint& point) const
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

QPoint Controller::keepInViewPort(const QPoint& point) const
{
    Vec3f dev_coord = m_InvDeviceMat * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    const QRectF& vp_size = m_ActiveViewPort->getSize();

    if(dev_coord(0) < vp_size.left())
        dev_coord(0) = vp_size.left();
    if(dev_coord(0) > vp_size.right())
        dev_coord(0) = vp_size.right();

    if(dev_coord(1) < vp_size.top())
        dev_coord(1) = vp_size.top();
    if(dev_coord(1) > vp_size.bottom())
        dev_coord(1) = vp_size.bottom();

    dev_coord(2) = 1.0;
    dev_coord = m_DeviceMat * dev_coord;

    return QPoint((int)(dev_coord(0) + 0.5f), (int)(dev_coord(1) + 0.5f));
}

void Controller::updateTransMatrix()
{
    Mat3f V = m_ActiveCamera->getViewMatrix();
    Mat3f P = m_ActiveViewPort->getProjectionMatrix();
    
    m_DeviceMat = m_RenderTarget->getDeviceMatrix();
    m_TransMat = m_DeviceMat * P * V;

    m_InvDeviceMat = m_DeviceMat.inverse();
    m_InvTransMat = m_TransMat.inverse();
}

QPointF Controller::trans(const QPointF& point)
{
    Vec3f trans_p = m_TransMat * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    return QPointF(trans_p.x(), trans_p.y());
}

QPointF Controller::invTrans(const QPointF& point)
{
    Vec3f trans_p = m_InvTransMat * Vec3f((float)point.x(), (float)point.y(), 1.0f);

    return QPointF(trans_p.x(), trans_p.y());
}
