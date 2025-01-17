#include "2DUniverse/Controller/QuadraticBezier/ControllerQuadraticBezier.h"

#include "2DUniverse/Rendering/RenderTarget.h"
#include "2DUniverse/Rendering/Renderer.h"

#include "2DUniverse/Scene/Scene.h"
#include "2DUniverse/Scene/ViewPort.h"
#include "2DUniverse/Scene/Camera.h"

#include "2DUniverse/Renderables/Point.h"
#include "2DUniverse/Renderables/QuadraticBezier.h"

#include "2DUniverse/HalfEdge/HESMesh.h"

#include "2DUniverse/Mesh/Vertex.h"

#include <QtGui/QMouseEvent>

ControllerQuadraticBezier::ControllerQuadraticBezier()
{
    m_Name = "ControllerQuadraticBezier";
}

ControllerQuadraticBezier::~ControllerQuadraticBezier()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Bezier;
}

void ControllerQuadraticBezier::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;
    m_Bezier = new QuadraticBezier();
    m_Bezier->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    m_Bezier->setVisible(false);

    // add to scene
    m_Scene->addQuadraticBeziers(m_Bezier);
}

void ControllerQuadraticBezier::activate()
{

}

void ControllerQuadraticBezier::deactivate()
{

}

bool ControllerQuadraticBezier::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i pos_in_vp = keepInViewPort(pos_global);
    Vec2d pos = Vec2d((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2d new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);

    m_Bezier->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

    m_Renderer->render();

    return true;
}

bool ControllerQuadraticBezier::handleMousePressEvent(QMouseEvent* const event)
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
    Vec2d p_f(p[0], p[1]);

    m_CurrentPoint = getPointAtPos(invTrans(p_f), &m_CurrentPointIdx);

    // if hit nothing and point size < 3 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 3)
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

        if(m_Points.size() == 3)
            m_Bezier->setVisible(true);
    }

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerQuadraticBezier::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerQuadraticBezier::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerQuadraticBezier::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerQuadraticBezier::getPointAtPos(const Vec2d& pos, size_t* const idx) const
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
