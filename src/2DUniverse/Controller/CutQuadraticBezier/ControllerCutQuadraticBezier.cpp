#include "2DUniverse/Controller/CutQuadraticBezier/ControllerCutQuadraticBezier.h"

#include "2DUniverse/Scene/Scene.h"

#include "2DUniverse/Rendering/Renderer.h"

#include "2DUniverse/Renderables/Point.h"
#include "2DUniverse/Renderables/Line.h"
#include "2DUniverse/Renderables/QuadraticBezier.h"

#include <QtGui/QMouseEvent>

ControllerCutQuadraticBezier::ControllerCutQuadraticBezier()
{
    m_Name = "ControllerCutQuadraticBezier";
}

ControllerCutQuadraticBezier::~ControllerCutQuadraticBezier()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Line;
    delete m_Bezier;

    delete m_CutPoint0;
    delete m_CutPoint1;
}

void ControllerCutQuadraticBezier::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Line = new Line();
    m_Bezier = new QuadraticBezier();
    m_CutPoint0 = new Point();
    m_CutPoint1 = new Point();

    m_Line->setVisible(false);
    m_Bezier->setVisible(false);
    m_CutPoint0->setVisible(false);
    m_CutPoint1->setVisible(false);

    m_Line->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));
    m_Bezier->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));
    m_CutPoint0->setColour(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));
    m_CutPoint1->setColour(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));

    m_CutPoint0->setSize(0.02f);
    m_CutPoint1->setSize(0.02f);

    // add to scene
    m_Scene->addLine(m_Line);
    m_Scene->addQuadraticBeziers(m_Bezier);
    m_Scene->addPoint(m_CutPoint0);
    m_Scene->addPoint(m_CutPoint1);
}

void ControllerCutQuadraticBezier::activate()
{

}

void ControllerCutQuadraticBezier::deactivate()
{

}

bool ControllerCutQuadraticBezier::handleMouseMoveEvent(QMouseEvent* const event)
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

    if(m_CurrentPointIdx == 0)
        m_Bezier->setPoint(0, m_CurrentPoint->getPosition());
    
    if(m_CurrentPointIdx == 1)
        m_Bezier->setPoint(1, m_CurrentPoint->getPosition());
    
    if(m_CurrentPointIdx == 2)
        m_Bezier->setPoint(2, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 3)
        m_Line->setPoint(0, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 4)
        m_Line->setPoint(1, m_CurrentPoint->getPosition());

    cut();

    m_Renderer->render();

    return true;
}

bool ControllerCutQuadraticBezier::handleMousePressEvent(QMouseEvent* const event)
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

    // if hit nothing and point size < 5 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 5)
        {
            m_MovePoint = false;

            return true;
        }

        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));
        
        m_CurrentPointIdx = m_Points.size();

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_CurrentPointIdx == 0)
            m_Bezier->setPoint(0, m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 1)
            m_Bezier->setPoint(1, m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 2)
        {
            m_Bezier->setPoint(2, m_CurrentPoint->getPosition());
            m_Bezier->setVisible(true);
        }

        if(m_CurrentPointIdx == 3)
            m_Line->setPoint(0, m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 4)
        {
            m_Line->setPoint(1, m_CurrentPoint->getPosition());
            m_Line->setVisible(true);
        }
    }

    cut();

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCutQuadraticBezier::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerCutQuadraticBezier::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutQuadraticBezier::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCutQuadraticBezier::getPointAtPos(const Vec2d& pos, size_t* const idx) const
{
    if(m_Points.empty())
        return nullptr;

    for(size_t i = 0; i < m_Points.size(); i++)
    {
        if((m_Points[i]->getPosition() - pos).norm() >= m_Points[i]->getSize())
            continue;

        *idx = i;

        return m_Points[i];
    }

    return nullptr;
}

void ControllerCutQuadraticBezier::cut()
{
    if(m_Points.size() != 5)
        return;

    // reset cut point
    m_CutPoint0->setVisible(false);
    m_CutPoint1->setVisible(false);

    IntersectionVector intersections = m_Line->intersect(m_Bezier);
    
    if(intersections.empty())
        return;
    if(intersections.size() >= 1)
    {
        m_CutPoint0->setVisible(true);
        m_CutPoint0->setPosition(intersections[0].m_Point);
    }
    if(intersections.size() >= 2)
    {
        m_CutPoint1->setVisible(true);
        m_CutPoint1->setPosition(intersections[1].m_Point);
    }
}
