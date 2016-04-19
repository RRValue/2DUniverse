#include "HalfEdge2D/Controller/CubicBezier/ControllerCubicBezier.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"

#include <QtGui/QMouseEvent>

ControllerCubicBezier::ControllerCubicBezier()
{
    m_Scene = nullptr;
    m_MovePoint = false;
    m_Name = "ControllerCubicBezier";
    m_CurrentPoint = nullptr;
    m_Bezier = new CubicBezier();
    m_Bezier->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    m_Bezier->setVisible(false);
}

ControllerCubicBezier::~ControllerCubicBezier()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Bezier;
}

void ControllerCubicBezier::activate()
{
    m_Scene->addCubicBeziers(m_Bezier);

    for(const auto& p : m_Points)
        m_Scene->addPoint(p);
}

void ControllerCubicBezier::deactivate()
{
    m_Scene->removeCubicBeziers(m_Bezier);

    for(const auto& p : m_Points)
        m_Scene->removePoint(p);
}

bool ControllerCubicBezier::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i pos_in_vp = keepInViewPort(pos_global);
    Vec2f pos = Vec2f((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2f new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);

    m_Bezier->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

    m_Renderer->render();

    return true;
}

bool ControllerCubicBezier::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(m_MovePoint)
        return false;

    if(m_ActiveViewPort == nullptr)
        return false;

    if(m_ActiveCamera == nullptr)
        return false;

    updateTransMatrix();

    // set to move mode
    if(event->button() == Qt::LeftButton)
        m_MovePoint = true;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i p = keepInViewPort(pos_global);
    Vec2f p_f(p[0], p[1]);

    m_CurrentPoint = getPointAtPos(invTrans(p_f), &m_CurrentPointIdx);

    // if hit nothing and point size < 4 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 4)
        {
            m_MovePoint = false;

            return true;
        }

        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));
        
        m_CurrentPointIdx = m_Points.size();

        m_Bezier->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_Points.size() == 4)
            m_Bezier->setVisible(true);
    }

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCubicBezier::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(event == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    m_MovePoint = false;

    return true;
}

bool ControllerCubicBezier::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCubicBezier::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCubicBezier::getPointAtPos(const Vec2f& pos, size_t* const idx) const
{
    if(m_Points.empty())
        return nullptr;

    // TODO point size from renderable point
    for(size_t i = 0; i < m_Points.size(); i++)
    {
        if((m_Points[i]->getPosition() - pos).norm() >= m_Points[i]->getSize())
            continue;

        *idx = i;

        return m_Points[i];
    }

    return nullptr;
}