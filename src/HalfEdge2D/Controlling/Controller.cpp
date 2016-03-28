#include "HalfEdge2D/Controlling/Controller.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include <QtGui/QMouseEvent>

Controller::Controller()
{
    m_ActiveCamera = nullptr;
    m_Name = "";
    m_Active = false;
    m_ViewportContentChanges = true;
    m_SceneChanges = false;
}

Controller::~Controller()
{

}

void Controller::setChanged()
{
    m_SceneChanges = true;
}

void Controller::preSceneChanged()
{

}

void Controller::postSceneChanged()
{

}

void Controller::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    if(scene == m_Scene)
        return;

    preSceneChanged();

    m_Scene = scene;

    postSceneChanged();
}

const bool& Controller::isActive() const
{
    return m_Active;
}

const std::string& Controller::getName() const
{
    return m_Name;
}

bool Controller::inViewPort(const Vec2i& point) const
{
    Mat3f inv_device_matrix = m_RenderTarget->getInvDeviceMatrix();
    Vec3f dev_coord = inv_device_matrix * Vec3f((float)point[0], (float)point[1], 1.0f);

    const QRectF& vp_size = m_ActiveViewPort->getSize();

    if(
        dev_coord(0) >= vp_size.x() && dev_coord(0) < vp_size.x() + vp_size.width() &&
        dev_coord(1) >= vp_size.y() && dev_coord(1) < vp_size.y() + vp_size.height())
        return true;

    return false;
}

Vec2i Controller::keepInViewPort(const Vec2i& point) const
{
    Vec3f dev_coord = m_InvDeviceMat * Vec3f((float)point[0], (float)point[1], 1.0f);

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

    return Vec2i((int)(dev_coord(0) + 0.5f), (int)(dev_coord(1) + 0.5f));
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

Vec2f Controller::trans(const Vec2f& point)
{
    Vec3f trans_p = m_TransMat * Vec3f(point[0], point[1], 1.0f);

    return Vec2f(trans_p[0], trans_p[1]);
}

Vec2f Controller::invTrans(const Vec2f& point)
{
    Vec3f trans_p = m_InvTransMat * Vec3f(point[0], point[1], 1.0f);

    return Vec2f(trans_p[0], trans_p[1]);
}
