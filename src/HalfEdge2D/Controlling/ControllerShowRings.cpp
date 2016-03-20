#include "HalfEdge2D/Controlling/ControllerShowRings.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Triangle.h"

#include <math.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QImage>

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
    m_Renderer->setRenderTrianglesEdges(false);
    m_Renderer->setSmoothRendering(false);
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
    updateIdTarget();

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

void ControllerShowRings::updateIdTarget()
{
    if(!m_ViewportContentChanges)
        return;

    m_ViewportContentChanges = false;

    if(m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr || m_RenderTarget == nullptr)
        return;

    m_ViewPort->setCamera(m_ActiveCamera);

    QSizeF render_target_size = m_RenderTarget->getSize();
    QRectF viewport_size = m_ActiveViewPort->getSize();
    QSizeF id_target_size(render_target_size.width() * viewport_size.width(), render_target_size.height() * viewport_size.height());

    m_IdTarget->resize((int)(id_target_size.width() + 0.5f), (int)(id_target_size.height() + 0.5f));
    
    // prepare mesh
    Mesh* mesh = m_Scene->getMesh();

    m_Triangles = mesh->getTriangles();

    if(m_Triangles.empty())
        return;

    Vec4f prev_triangle_color = m_Triangles[0]->getColor();

    int color_step = 8;
    int color_step_value = (int)std::pow(2.0, (float)color_step);
    float color_factor = 1.0f / (float)color_step_value;
    int r = 0;
    int g = 0;
    int b = 0;
    Vec4f current_color;

    for(size_t i = 0; i < m_Triangles.size(); i++)
    {
        r = (i >> 0 * color_step) % color_step_value;
        g = (i >> 1 * color_step) % color_step_value;
        b = (i >> 2 * color_step) % color_step_value;

        current_color = Vec4f(
            color_factor * (float)r,
            color_factor * (float)g,
            color_factor * (float)b,
            1.0f);

        m_Triangles[i]->setColor(current_color);
    }

    m_Renderer->render(m_IdTarget);

    for(size_t i = 0; i < m_Triangles.size(); i++)
        m_Triangles[i]->setColor(prev_triangle_color);

    m_IdTarget->save("image.png");
}
