#include "HalfEdge2D/Controller/CutMeshLine/ControllerCutMeshLine.h"

#include "HalfEdge2D/Controller/CutMesh/CutMeshOption_uic.h"

#include <QtGui/QMouseEvent>

ControllerCutMeshLine::ControllerCutMeshLine()
{
    m_Name = "ControllerCutMeshLine";
}

ControllerCutMeshLine::~ControllerCutMeshLine()
{

}

void ControllerCutMeshLine::init()
{

}

void ControllerCutMeshLine::activate()
{

}

void ControllerCutMeshLine::deactivate()
{

}

bool ControllerCutMeshLine::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    return true;
}

bool ControllerCutMeshLine::handleMousePressEvent(QMouseEvent* const event)
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

bool ControllerCutMeshLine::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(event == nullptr)
        return false;

    return true;
}

bool ControllerCutMeshLine::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutMeshLine::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}
