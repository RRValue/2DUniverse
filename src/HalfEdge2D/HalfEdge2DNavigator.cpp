#include "HalfEdge2D/HalfEdge2DNavigator.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/Canvas.h"

#include <QtGui/QMouseEvent>

#include <qdebug.h>

HalfEdge2DNavigator::HalfEdge2DNavigator(HalfEdge2DWidget* const widget) : m_Widget(widget)
{
    m_Navigatin = false;
    m_Scene = nullptr;
    m_ActiveCamera = nullptr;
    m_ZoomFactor = 1.05f;
}

HalfEdge2DNavigator::~HalfEdge2DNavigator()
{

}

void HalfEdge2DNavigator::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    m_Scene = scene;
}

void HalfEdge2DNavigator::updateCameraInformation()
{
    if(m_Scene == nullptr)
        return;

    m_ActiveCamera = m_Scene->getCamera();
}

bool HalfEdge2DNavigator::handleMousePressEvent(QMouseEvent* const event)
{
    updateCameraInformation();

    if(m_ActiveCamera == nullptr)
        return false;

    if(m_Navigatin)
        return false;

    if(event->button() == Qt::RightButton)
        m_Navigatin = true;

    if(!m_Navigatin)
        return false;

    QPoint pos_px = event->pos();

    if(!m_Scene->inCanvas(pos_px))
        m_Navigatin = false;

    if(!m_Navigatin)
        return false;

    m_CamMoveInitMousePos = m_Scene->invTransform(pos_px);
    m_CamMoveInitCamPos = m_ActiveCamera->getPosition();

    qDebug() << "Start Navigate";

    return true;
}

bool HalfEdge2DNavigator::handleMouseMoveEvent(QMouseEvent* const event)
{
    updateCameraInformation();

    if(m_ActiveCamera == nullptr)
        return false;

    if(!m_Navigatin)
        return false;

    m_ActiveCamera->setPosition(m_CamMoveInitCamPos);

    QPoint pos = m_Scene->keepInCanvas(event->pos());
    QPointF current_delta = m_CamMoveInitMousePos - m_Scene->invTransform(pos);

    m_ActiveCamera->setPosition(m_CamMoveInitCamPos + current_delta);

    // update widget
    m_Widget->update();

    qDebug() << "Navigate";

    return true;
}

bool HalfEdge2DNavigator::handleMouseReleaseEvent(QMouseEvent* const event)
{
    updateCameraInformation();

    if(m_ActiveCamera == nullptr)
        return false;

    if(!m_Navigatin)
        return false;

    m_Navigatin = false;

    qDebug() << "Stop Navigate";

    return true;
}

bool HalfEdge2DNavigator::handleResizeEvent(QResizeEvent* const event)
{
    updateCameraInformation();

    if(m_ActiveCamera == nullptr)
        return false;

    m_ActiveCamera->getCanvas()->setSize(event->size());

    qDebug() << "Resize";

    return true;
}

bool HalfEdge2DNavigator::handleWheelEvent(QWheelEvent* const event)
{
    updateCameraInformation();

    if(m_ActiveCamera == nullptr)
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

    QPointF mouse_pos = m_Scene->invTransform(pos_px);

    // adjust zoom
    QPointF ortho_size = m_ActiveCamera->getOrthoSize();

    if(step > 0)
        ortho_size *= float(step) * m_ZoomFactor;
    else
        ortho_size /= std::abs(float(step)) * m_ZoomFactor;

    m_ActiveCamera->setOrthoSize(ortho_size);

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
    m_Widget->update();
}
