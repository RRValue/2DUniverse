#include "HalfEdge2D/Controlling/ControllerShowRings.h"


ControllerShowRings::ControllerShowRings()
{
    m_Mesh = nullptr;
    m_Name = "ControllerShowRings";
}

ControllerShowRings::~ControllerShowRings()
{

}

void ControllerShowRings::setMesh(HESMesh* const mesh)
{
    if(mesh == nullptr)
        return;

    if(mesh == m_Mesh)
        return;

    m_Mesh = mesh;
}

bool ControllerShowRings::handleMouseMoveEvent(QMouseEvent* const event)
{
    return false;
}

bool ControllerShowRings::handleMousePressEvent(QMouseEvent* const event)
{
    return false;
}

bool ControllerShowRings::handleMouseReleaseEvent(QMouseEvent* const event)
{
    return false;
}

bool ControllerShowRings::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerShowRings::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}
