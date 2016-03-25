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
m_ChannelBitRange(6),
m_ChannelRange(1 << m_ChannelBitRange),
m_ChannelFFactor(1.0f / (float)m_ChannelRange),
m_MaxId(m_ChannelBitRange * m_ChannelBitRange * m_ChannelBitRange),
m_NumRings(1)
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
    m_LastColourVector.clear();
}

void ControllerShowRings::setMesh(HESMesh* const mesh)
{
    if(mesh == nullptr)
        return;

    clear();

    m_Mesh = mesh;

    m_Scene->setMesh(m_Mesh);
    m_Faces = mesh->getFaces();

    m_ViewportContentChanges = true;
    m_LastHitId = m_MaxId;
}

bool ControllerShowRings::handleMouseMoveEvent(QMouseEvent* const event)
{
    updateIdTarget();

    if(m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr || m_RenderTarget == nullptr)
        return false;

    Vec3f hit_px_pos = m_MousePosToTargetMat * Vec3f((float)event->x(), (float)event->y(), 1.0f);

    // get colur id_target
    Vec4f hit_colour = m_IdTarget->getColourAtPos((int)(hit_px_pos[0] + 0.5), (int)(hit_px_pos[1] + 0.5));

    size_t current_hit_id = m_MaxId;

    if(!(hit_colour[0] == 1.0f, hit_colour[1] == 1.0f, hit_colour[2] == 1.0f))
        current_hit_id = colourToId(hit_colour);

    if(current_hit_id == m_MaxId || current_hit_id >= m_Faces.size())
        return true;

    if(m_LastHitId == current_hit_id)
        return true;

    HESFace* center_face = dynamic_cast<HESFace*>(m_Faces[current_hit_id]);

    if(center_face == nullptr)
        return true;

    m_LastHitId = current_hit_id;

    unsetLastFaces();
    colourRingFaces(findRingOrder(center_face, m_NumRings));

    m_Renderer->render();

    return true;
}

void ControllerShowRings::unsetLastFaces()
{
    for(const auto& last_colour : m_LastColourVector)
        last_colour.m_Face->setColor(last_colour.m_Colour);
}

void ControllerShowRings::colourRingFaces(const RingFacesVector& rings)
{
    m_LastColourVector.clear();

    if(rings.empty())
        return;

    std::vector<float> r_value;
    std::vector<float> b_value;

    float r_step = 1.0f / ((float)rings.size() - 1);
    float b_step = 1.0f / ((float)rings.size() - 1);
    
    for(size_t i = 0; i < rings.size(); i++)
    {
        r_value.push_back(r_step * (float)i);
        b_value.push_back(b_step * (float)i);
    }
    
    // colour ring faces
    size_t idx = 0;
    float r, g, b;
    for(auto ring : rings)
    {
        for(const auto& face : ring)
        {
            m_LastColourVector.push_back({face, face->getColor()});
            
            r = 1.0f - b_value[idx];
            g = r_value[idx] + (1.0f - b_value[idx]);
            b = r_value[idx];

            face->setColor(Vec4f(r, g, b, 1.0f));
        }

        idx++;
    }
}

RingFacesVector ControllerShowRings::findRingOrder(HESFace* const face, const size_t& order)
{
    RingFacesVector ring_faces;

    if(face == nullptr)
        return ring_faces;

    size_t current_order = 0;

    // add order 0
    ring_faces.push_back(FaceVector());
    ring_faces[current_order].push_back(face);
    
    face->setVisited(true);

    current_order++;

    // add order 1 .. n
    while(current_order <= order)
    {
        ring_faces.push_back(FaceVector());

        for(const auto& last_face : ring_faces[current_order - 1])
        {
            for(const auto& face_edge : last_face->getEdges())
            {
                for(const auto& edge : face_edge->from()->getEdges())
                {
                    HESFace* const face = edge->face();

                    if(face == nullptr)
                        continue;

                    if(face->visited())
                        continue;

                    ring_faces[current_order].push_back(face);

                    face->setVisited(true);
                }
            }
        }

        if(ring_faces[current_order].empty())
        {
            ring_faces.erase(ring_faces.begin() + ring_faces.size() - 1);

            break;
        }

        current_order++;
    }

    // set all unvisited
    for(const auto& ring : ring_faces)
        for(const auto& face : ring)
            face->setVisited(false);

    return ring_faces;
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

    // draw id mesh
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
