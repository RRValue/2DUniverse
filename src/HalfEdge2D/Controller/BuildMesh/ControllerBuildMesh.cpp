#include "HalfEdge2D/Controller/BuildMesh/ControllerBuildMesh.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"

#include <QtGui/QMouseEvent>

ControllerBuildMesh::ControllerBuildMesh()
{
    m_Scene = nullptr;
    m_MovePoint = false;
    m_Name = "ControllerBuildMesh";
}

ControllerBuildMesh::~ControllerBuildMesh()
{

}

void ControllerBuildMesh::activate()
{

}

void ControllerBuildMesh::deactivate()
{

}

bool ControllerBuildMesh::handleMouseMoveEvent(QMouseEvent* const event)
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
    
    (*m_Scene->getMeshes().begin())->getVertices()[m_CurrentIdx]->setPosition(new_pos);
    
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

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i p = keepInViewPort(pos_global);
    Vec2f p_f(p[0], p[1]);

    int result = getPointAtPos(invTrans(p_f));

    // if hit nothing and point size < 4 -> add
    if(result == -1)
    {
        (*m_Scene->getMeshes().begin())->addVertex(invTrans(p_f));

        m_MovePoint = false;

        m_Renderer->render();
    }
    else
    {
        m_CurrentIdx = result;
        
        m_CurrentHitDistance = trans((*m_Scene->getMeshes().begin())->getVertices()[m_CurrentIdx]->getPosition()) - p_f;
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

int ControllerBuildMesh::getPointAtPos(const Vec2f& pos) const
{
    if(m_Scene->getMeshes().empty())
        return -1;

    const std::vector<Vertex*>& vertices = (*m_Scene->getMeshes().begin())->getVertices();

    if(vertices.empty())
        return -1;

    // TODO point size from renderable point
    for(size_t i = 0; i < vertices.size(); i++)
        if((vertices[i]->getPosition() - pos).norm() < 0.05f)
            return i;

    return -1;
}