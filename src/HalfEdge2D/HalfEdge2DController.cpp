#include "HalfEdge2D/HalfEdge2DController.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include "HalfEdge2D/Scene/Scene.h"

#include <QtGui/QMouseEvent>

HalfEdge2DController::HalfEdge2DController(HalfEdge2DWidget* const widget) : m_Widget(widget)
{
    m_Mesh = new HESMesh();

    m_Mesh->addVertex();
    m_Mesh->addVertex();
    m_Mesh->addVertex();
    m_Mesh->addVertex();
    m_Mesh->addVertex();

    m_Mesh->addTriangle(0, 1, 2);
    m_Mesh->addTriangle(0, 2, 3);
    m_Mesh->addTriangle(4, 3, 2);

    HESBuilder builder(m_Mesh);
    builder.build();

    m_Scene = nullptr;
    m_MovePoint = false;
}

HalfEdge2DController::~HalfEdge2DController()
{
    delete m_Mesh;
}

void HalfEdge2DController::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    m_Scene = scene;
}

bool HalfEdge2DController::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    QPointF pos = m_Scene->keepInCanvas(event->pos()) + m_CurrentHitDistance;
    m_Scene->setPointPos(m_CurrentIdx, m_Scene->invTransform(pos));
    
    return true;
}

bool HalfEdge2DController::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(m_MovePoint)
        return false;

    // set to move mode
    if(event->button() == Qt::LeftButton)
        m_MovePoint = true;
    
    if(!m_MovePoint)
        return false;

    QPointF p = m_Scene->keepInCanvas(event->pos());

    int result = m_Scene->getPointAtPos(p);

    // if hit nothing and point size < 4 -> add
    if(result == -1)
    {
        m_Scene->addPoint(m_Scene->invTransform(p));

        m_MovePoint = false;

        m_Widget->update();
    }
    else
    {
        m_CurrentIdx = result;
        m_CurrentHitDistance = m_Scene->transform(m_Scene->getPoint(m_CurrentIdx)) - p;
    }

    return true;
}

bool HalfEdge2DController::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool HalfEdge2DController::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool HalfEdge2DController::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}
