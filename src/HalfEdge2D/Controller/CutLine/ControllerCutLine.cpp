#include "HalfEdge2D/Controller/CutLine/ControllerCutLine.h"

#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"

#include <QtGui/QMouseEvent>

ControllerCutLine::ControllerCutLine()
{
    m_Name = "ControllerCutLine";
}

ControllerCutLine::~ControllerCutLine()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Line0;
    delete m_Line1;

    delete m_CutPoint;
}

void ControllerCutLine::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Line0 = new Line();
    m_Line1 = new Line();
    m_CutPoint = new Point();

    m_Line0->setVisible(false);
    m_Line1->setVisible(false);
    m_CutPoint->setVisible(false);

    m_Line0->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));
    m_Line1->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));
    m_CutPoint->setColour(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));

    m_CutPoint->setSize(0.02f);

    // add to scene
    m_Scene->addLine(m_Line0);
    m_Scene->addLine(m_Line1);
    m_Scene->addPoint(m_CutPoint);
}

void ControllerCutLine::activate()
{

}

void ControllerCutLine::deactivate()
{

}

bool ControllerCutLine::handleMouseMoveEvent(QMouseEvent* const event)
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
        m_Line0->setPoint(0, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 1)
        m_Line0->setPoint(1, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 2)
        m_Line1->setPoint(0, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 3)
        m_Line1->setPoint(1, m_CurrentPoint->getPosition());

    cut();

    m_Renderer->render();

    return true;
}

bool ControllerCutLine::handleMousePressEvent(QMouseEvent* const event)
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

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_CurrentPointIdx == 0)
            m_Line0->setPoint(0, m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 1)
        {
            m_Line0->setPoint(1, m_CurrentPoint->getPosition());
            m_Line0->setVisible(true);
        }

        if(m_CurrentPointIdx == 2)
            m_Line1->setPoint(0, m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 3)
        {
            m_Line1->setPoint(1, m_CurrentPoint->getPosition());
            m_Line1->setVisible(true);
        }
    }

    cut();

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCutLine::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerCutLine::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutLine::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCutLine::getPointAtPos(const Vec2d& pos, size_t* const idx) const
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

void ControllerCutLine::cut()
{
    if(m_Points.size() != 4)
        return;

    // reset cut point
    m_CutPoint->setVisible(false);

    IntersectionVector intersections = m_Line0->intersect(m_Line1);

    if(intersections.empty())
        return;

    m_CutPoint->setVisible(true);
    m_CutPoint->setPosition(intersections[0].m_Point);
}
