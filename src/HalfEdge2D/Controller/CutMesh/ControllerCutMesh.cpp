#include "HalfEdge2D/Controller/CutMesh/ControllerCutMesh.h"

#include "HalfEdge2D/Controller/CutMesh/CutMeshOption_uic.h"

#include <QtGui/QMouseEvent>

ControllerCutMesh::ControllerCutMesh()
{
    m_Name = "ControllerCutMesh";
}

ControllerCutMesh::~ControllerCutMesh()
{

}

void ControllerCutMesh::init()
{

}

void ControllerCutMesh::activate()
{

}

void ControllerCutMesh::deactivate()
{

}

bool ControllerCutMesh::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    return true;
}

bool ControllerCutMesh::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;


    if(m_ActiveViewPort == nullptr)
        return false;

    if(m_ActiveCamera == nullptr)
        return false;

    updateTransMatrix();

    return true;
}

bool ControllerCutMesh::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(event == nullptr)
        return false;

    return true;
}

bool ControllerCutMesh::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutMesh::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}
