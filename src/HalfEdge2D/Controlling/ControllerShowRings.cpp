#include "HalfEdge2D/Controlling/ControllerShowRings.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include <QtGui/QMouseEvent>

#include <qdebug.h>

ControllerShowRings::ControllerShowRings()
{
    m_Name = "ControllerShowRings";

    m_Scene = new Scene();
    m_Mesh = nullptr;

    m_Renderer = new Renderer();
    m_IdTarget = new QPaintTarget(100, 100);

    m_ViewPort = new ViewPort();

    m_IdTarget->addViewPort(m_ViewPort);
    m_Renderer->setScene(m_Scene);
    m_Renderer->setRenderViewport(false);
    m_Renderer->setRenderCoordianteAxis(false);
    m_Renderer->setRenderVertices(false);
    m_Renderer->setRenderTriangles(true);
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

    m_Scene->setMesh(m_Mesh);
}

bool ControllerShowRings::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(!m_ViewportContentChanges)
        return false;

    m_ViewportContentChanges = false;

    if(m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;
    
    m_ViewPort->setSize(m_ActiveViewPort->getSize());
    m_ViewPort->setCamera(m_ActiveCamera);

    m_Renderer->render(m_IdTarget);

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
