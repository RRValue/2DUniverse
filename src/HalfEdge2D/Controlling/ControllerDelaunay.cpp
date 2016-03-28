#include "HalfEdge2D/Controlling/ControllerDelaunay.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include "HalfEdge2D/Renderables/Point.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"

#include <QtGui/QMouseEvent>

ControllerDelaunay::ControllerDelaunay()
{
    m_Scene = nullptr;
    m_MovePoint = false;
    m_Name = "ControllerDelaunay";
    m_CurrentPoint = nullptr;
}

ControllerDelaunay::~ControllerDelaunay()
{

}

bool ControllerDelaunay::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i pos_in_vp = keepInViewPort(pos_global);
    Vec2f pos = Vec2f((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2f new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);
    
    m_Scene->setPoints(m_Points);

    m_Renderer->render();

    return true;
}

bool ControllerDelaunay::handleMousePressEvent(QMouseEvent* const event)
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

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i p = keepInViewPort(pos_global);
    Vec2f p_f(p[0], p[1]);

    m_CurrentPoint = getPointAtPos(invTrans(p_f));

    // if hit nothing and point size < 4 -> add
    if(m_CurrentPoint == nullptr)
    {
        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));
        
        m_MovePoint = false;

        m_Points.insert(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);
        
        m_Renderer->render();
    }
    
    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerDelaunay::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerDelaunay::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerDelaunay::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerDelaunay::getPointAtPos(const Vec2f& pos) const
{
    if(m_Points.empty())
        return nullptr;

    // TODO point size from renderable point
    for(const auto& p : m_Points)
        if((p->getPosition() - pos).norm() < p->getSize())
            return p;

    return nullptr;
}
