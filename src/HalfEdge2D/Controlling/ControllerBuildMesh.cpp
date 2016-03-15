#include "HalfEdge2D/Controlling/ControllerBuildMesh.h"

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
    m_Mesh = nullptr;
    m_MovePoint = false;
}

ControllerBuildMesh::~ControllerBuildMesh()
{

}

void ControllerBuildMesh::setMesh(HESMesh* const mesh)
{
    if(mesh == nullptr)
        return;

    if(mesh == m_Mesh)
        return;

    m_Mesh = mesh;
}

bool ControllerBuildMesh::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_Mesh == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    QPointF pos = keepInViewPort(event->pos()) + m_CurrentHitDistance;
    QPointF new_pos = invTrans(pos);

    Vec2f(new_pos.x(), new_pos.y());
    
    m_Mesh->getVertices()[m_CurrentIdx]->setPosition(Vec2f(new_pos.x(), new_pos.y()));
    
    m_Renderer->render();
    
    return true;
}

bool ControllerBuildMesh::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Mesh == nullptr)
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



    int result = getPointAtPos(invTrans(p));

    // if hit nothing and point size < 4 -> add
    if(result == -1)
    {
        QPointF trans_pos = invTrans(p);
        Vec2f vert_pos(trans_pos.x(), trans_pos.y());

        m_Mesh->addVertex(vert_pos);

        m_MovePoint = false;

        m_Renderer->render();
    }
    else
    {
        m_CurrentIdx = result;

        Vec2f vert_pos = m_Mesh->getVertices()[m_CurrentIdx]->getPosition();
        
        m_CurrentHitDistance = trans(QPointF(vert_pos.x(), vert_pos.y())) - p;
    }

    return true;
}

bool ControllerBuildMesh::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Mesh == nullptr)
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

int ControllerBuildMesh::getPointAtPos(const QPointF& pos) const
{
    const std::vector<Vertex*>& vertices = m_Mesh->getVertices();

    if(vertices.empty())
        return -1;

    for(size_t i = 0; i < vertices.size(); i++)
    {
        const Vec2f& vert_pos = vertices[i]->getPosition();

        QPointF point_pos(vert_pos.x(), vert_pos.y());
        QPointF delta = point_pos - pos;
        float distance = std::sqrt(std::pow(delta.x(), 2) + std::pow(delta.y(), 2));

        // TODO point size from renderable point
        if(distance < 0.05f)
            return i;
    }

    return -1;
}
