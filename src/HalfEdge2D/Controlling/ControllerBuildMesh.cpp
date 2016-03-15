#include "HalfEdge2D/Controlling/ControllerBuildMesh.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include <QtGui/QMouseEvent>

ControllerBuildMesh::ControllerBuildMesh()
{
    m_MovePoint = false;
}

ControllerBuildMesh::~ControllerBuildMesh()
{

}

void ControllerBuildMesh::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    if(scene == m_Scene)
        return;

    m_Scene = scene;
}

bool ControllerBuildMesh::handleMouseMoveEvent(QMouseEvent* const event)
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

bool ControllerBuildMesh::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(m_MovePoint)
        return false;

    if(m_ActiveViewPort == nullptr)
        return false;

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

bool ControllerBuildMesh::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerBuildMesh::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerBuildMesh::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}
