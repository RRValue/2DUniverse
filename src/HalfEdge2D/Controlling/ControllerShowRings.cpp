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

ControllerShowRings::ControllerShowRings() :
m_ChannelBitRange(8),
m_ChannelRange(1 << m_ChannelBitRange),
m_ChannelFFactor(1.0f / (float)m_ChannelRange)
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

    if(m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr || m_RenderTarget == nullptr)
        return false;

    Mat3f target_dev_matrix = m_RenderTarget->getDeviceMatrix();
    Mat3f id_dev_matrix = m_IdTarget->getInvDeviceMatrix();

    Vec3f hit_px_pos = id_dev_matrix * target_dev_matrix * Vec3f((float)event->x(), (float)event->y(), 1.0f);

    // get colur id_target
    Vec4f hit_colour = m_IdTarget->getColourAtPos((int)(hit_px_pos[0] + 0.5), (int)(hit_px_pos[1] + 0.5));

    unsigned int id = colourToId(hit_colour);

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

    for(size_t i = 0; i < m_Triangles.size(); i++)
        m_Triangles[i]->setColor(idToColour(i));

    m_Renderer->render(m_IdTarget);

    for(size_t i = 0; i < m_Triangles.size(); i++)
        m_Triangles[i]->setColor(prev_triangle_color);

    m_IdTarget->save("image.png");
}

Vec4f ControllerShowRings::idToColour(const unsigned int& id)
{
    return Vec4f(
        m_ChannelFFactor * (float)((id >> 0 * m_ChannelBitRange) % m_ChannelRange),
        m_ChannelFFactor * (float)((id >> 1 * m_ChannelBitRange) % m_ChannelRange),
        m_ChannelFFactor * (float)((id >> 2 * m_ChannelBitRange) % m_ChannelRange),
        1.0f);
}

unsigned int ControllerShowRings::colourToId(const Vec4f& colour)
{
    return
        (((unsigned int)((colour[0] / m_ChannelFFactor) + 0.5f)) << (0 * m_ChannelBitRange)) +
        (((unsigned int)((colour[1] / m_ChannelFFactor) + 0.5f)) << (1 * m_ChannelBitRange)) +
        (((unsigned int)((colour[2] / m_ChannelFFactor) + 0.5f)) << (2 * m_ChannelBitRange));
}
