#include "HalfEdge2D/HalfEdge2DController.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

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

    m_MovePoint = false;
    m_PointSizePx = 5.0f;
}

HalfEdge2DController::~HalfEdge2DController()
{
    delete m_Mesh;
}

bool HalfEdge2DController::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(!m_MovePoint)
        return false;

    QPointF pos = keepInWidget(event->pos()) + m_CurrentHitDistance;
    m_Points[m_CurrentIdx] = invTransform(pos);

    return true;
}

bool HalfEdge2DController::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_MovePoint)
        return false;

    // set to move mode
    if(event->button() == Qt::LeftButton)
        m_MovePoint = true;
    
    if(!m_MovePoint)
        return false;

    QPointF p = keepInWidget(event->pos());

    int result = getHitPoint(p);

    // if hit nothing and point size < 4 -> add
    if(result == -1)
    {
        m_Points.push_back(invTransform(p));

        m_MovePoint = false;

        m_Widget->update();
    }
    else
    {
        m_CurrentIdx = result;
        m_CurrentHitDistance = transform(m_Points[m_CurrentIdx]) - p;
    }

    return true;
}

bool HalfEdge2DController::handleMouseReleaseEvent(QMouseEvent* const event)
{
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

int HalfEdge2DController::getHitPoint(const QPointF& pos)
{
    if(m_Points.empty())
        return -1;

    for(int i = 0; i < m_Points.size(); i++)
    {
        QPointF point_pos = transform(m_Points[i]);
        QPointF delta = point_pos - pos;
        float distance = std::sqrt(std::pow(delta.x(), 2) + std::pow(delta.y(), 2));

        if(distance < m_PointSizePx)
            return i;
    }

    return -1;
}
