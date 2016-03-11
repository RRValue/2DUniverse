#include "HalfEdge2D/HalfEdge2DController.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

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
}

HalfEdge2DController::~HalfEdge2DController()
{
    delete m_Mesh;
}

bool HalfEdge2DController::handleMouseMoveEvent(QMouseEvent* const event)
{
    return true;
}

bool HalfEdge2DController::handleMousePressEvent(QMouseEvent* const event)
{
    return true;
}

bool HalfEdge2DController::handleMouseReleaseEvent(QMouseEvent* const event)
{
    return true;
}

bool HalfEdge2DController::handleResizeEvent(QResizeEvent* const event)
{
    return true;
}

bool HalfEdge2DController::handleWheelEvent(QWheelEvent* const event)
{
    return true;
}
