#include "HalfEdge2D/Controlling/ControllerShowRings.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESVertex.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Face.h"

#include <math.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QImage>

#include <qdebug.h>

ControllerShowRings::ControllerShowRings() :
m_ChannelBitRange(8),
m_ChannelRange(1 << m_ChannelBitRange),
m_ChannelFFactor(1.0f / (float)m_ChannelRange),
m_FaceHitColour(Vec4f(0.5f, 0.0f, 0.0f, 1.0f)),
m_FaceRingColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f))
{
    m_Name = "ControllerShowRings";

    m_Scene = new Scene();
    m_Mesh = nullptr;

    m_IdRenderer = new Renderer();
    m_IdTarget = new QPaintTarget(100, 100);

    m_ViewPort = new ViewPort();

    m_IdTarget->addViewPort(m_ViewPort);
    m_IdRenderer->setScene(m_Scene);
    m_IdRenderer->setRenderViewport(false);
    m_IdRenderer->setRenderCoordianteAxis(false);
    m_IdRenderer->setRenderVertices(false);
    m_IdRenderer->setRenderTriangles(true);
    m_IdRenderer->setRenderTrianglesEdges(false);
    m_IdRenderer->setSmoothRendering(false);
}

ControllerShowRings::~ControllerShowRings()
{

}

void ControllerShowRings::clear()
{
    m_LastFacesColours.clear();
    m_LastFaces.clear();
}

void ControllerShowRings::setMesh(HESMesh* const mesh)
{
    if(mesh == nullptr)
        return;

    clear();

    m_Mesh = mesh;

    m_Scene->setMesh(m_Mesh);
}

bool ControllerShowRings::handleMouseMoveEvent(QMouseEvent* const event)
{
    updateIdTarget();

    if(m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr || m_RenderTarget == nullptr)
        return false;

    Vec3f hit_px_pos = m_MousePosToTargetMat * Vec3f((float)event->x(), (float)event->y(), 1.0f);

    // get colur id_target
    Vec4f hit_colour = m_IdTarget->getColourAtPos((int)(hit_px_pos[0] + 0.5), (int)(hit_px_pos[1] + 0.5));

    int current_hit_id = -1;

    if(!(hit_colour[0] == 1.0f, hit_colour[1] == 1.0f, hit_colour[2] == 1.0f))
        current_hit_id = colourToId(hit_colour);

    if(current_hit_id == -1 || (size_t)current_hit_id >= m_Faces.size())
        return true;

    HESFace* center_face = dynamic_cast<HESFace*>(m_Faces[current_hit_id]);

    if(center_face == nullptr)
        return true;

    unsetLastFaces();
    setNeigbourFaces(center_face, findRing(center_face));

    m_Renderer->render();

    return true;
}

void ControllerShowRings::unsetLastFaces()
{
    for(const auto& face : m_LastFaces)
    {
        const auto& find_color = m_LastFacesColours.find(face);

        if(find_color == m_LastFacesColours.end())
            continue;

        find_color->first->setColor(find_color->second);
    }
}

void ControllerShowRings::setNeigbourFaces(HESFace* const centerface, const std::set<HESFace* const>& ringFaces)
{
    m_LastFacesColours.clear();
    m_LastFaces.clear();

    // colour center face
    m_LastFacesColours.insert(std::make_pair(centerface, centerface->getColor()));
    m_LastFaces.insert(centerface);

    centerface->setColor(m_FaceHitColour);

    // colour ring faces
    for(auto face : ringFaces)
    {
        m_LastFacesColours.insert(std::make_pair(face, face->getColor()));
        m_LastFaces.insert(face);

        face->setColor(m_FaceRingColour);
    }
}

std::set<HESFace* const> ControllerShowRings::findRing(HESFace* const face)
{
    std::set<HESFace* const> neigbour_tris;

    if(face == nullptr)
        return neigbour_tris;

    std::set<HESFace* const> visited;
    visited.insert(face);

    for(const auto& face_edge : face->getEdges())
    {
        for(const auto& edge : face_edge->from()->getEdges())
        {
            HESFace* const face = edge->face();

            if(face == nullptr)
                continue;

            if(visited.find(face) != visited.end())
                continue;

            visited.insert(face);
            neigbour_tris.insert(face);
        }
    }

    return neigbour_tris;
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
    m_ViewportContentChanges = true;

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

    // update mouse to target matrix
    Mat3f targetVp_to_idVp_mat;

    float ivpw = 1.0f / viewport_size.width();
    float ivph = 1.0f / viewport_size.height();

    targetVp_to_idVp_mat <<
        ivpw, 0.0f, -viewport_size.x() * ivpw,
        0.0f, ivph, -viewport_size.y() * ivph,
        0.0f, 0.0f, 1.0f;
  
    m_MousePosToTargetMat = m_IdTarget->getDeviceMatrix() * targetVp_to_idVp_mat * m_RenderTarget->getInvDeviceMatrix();

    // prepare mesh
    Mesh* mesh = m_Scene->getMesh();

    m_Faces = mesh->getFaces();

    if(m_Faces.empty())
        return;

    Vec4f prev_triangle_color = m_Faces[0]->getColor();

    for(size_t i = 0; i < m_Faces.size(); i++)
        m_Faces[i]->setColor(idToColour(i));

    m_IdRenderer->render(m_IdTarget);

    for(size_t i = 0; i < m_Faces.size(); i++)
        m_Faces[i]->setColor(prev_triangle_color);
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
