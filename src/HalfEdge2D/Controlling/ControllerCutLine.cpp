#include "HalfEdge2D/Controlling/ControllerCutLine.h"

#include "HalfEdge2D/Base/LineSegment.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"

#include <QtGui/QMouseEvent>

ControllerCutLine::ControllerCutLine()
{
    m_Scene = nullptr;
    m_MovePoint = false;
    m_Name = "ControllerCutLine";
    m_CurrentPoint = nullptr;

    m_Line0 = new Line();
    m_Line1 = new Line();
    m_LineTrans0 = new Line();
    m_LineTrans1 = new Line();

    m_Line0->setVisible(false);
    m_Line1->setVisible(false);
    m_LineTrans0->setVisible(false);
    m_LineTrans1->setVisible(false);

    m_Line0->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    m_Line1->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    m_LineTrans0->setColour(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    m_LineTrans1->setColour(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
}

ControllerCutLine::~ControllerCutLine()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Line0;
    delete m_Line1;
    delete m_LineTrans0;
    delete m_LineTrans1;
}

void ControllerCutLine::activate()
{
    m_Scene->addLine(m_Line0);
    m_Scene->addLine(m_Line1);
    m_Scene->addLine(m_LineTrans0);
    m_Scene->addLine(m_LineTrans1);

    for(const auto& p : m_Points)
        m_Scene->addPoint(p);
}

void ControllerCutLine::deactivate()
{
    m_Scene->removeLine(m_Line0);
    m_Scene->removeLine(m_Line1);
    m_Scene->removeLine(m_LineTrans0);
    m_Scene->removeLine(m_LineTrans1);

    for(const auto& p : m_Points)
        m_Scene->removePoint(p);
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
    Vec2f pos = Vec2f((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2f new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);

    if(m_CurrentPointIdx == 0)
        m_Line0->setPositionStart(m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 1)
        m_Line0->setPositionEnd(m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 2)
        m_Line1->setPositionStart(m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 3)
        m_Line1->setPositionEnd(m_CurrentPoint->getPosition());

    updateTransformedLines();

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

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_CurrentPointIdx == 0)
            m_Line0->setPositionStart(m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 1)
        {
            m_Line0->setPositionEnd(m_CurrentPoint->getPosition());
            m_Line0->setVisible(true);
        }

        if(m_CurrentPointIdx == 2)
            m_Line1->setPositionStart(m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 3)
        {
            m_Line1->setPositionEnd(m_CurrentPoint->getPosition());
            m_Line1->setVisible(true);
        }
    }

    if(m_Points.size() == 4)
    {
        m_LineTrans0->setVisible(true);
        m_LineTrans1->setVisible(true);
    }

    updateTransformedLines();

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

Point* const ControllerCutLine::getPointAtPos(const Vec2f& pos, size_t* const idx) const
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

void ControllerCutLine::updateTransformedLines()
{
    if(m_Points.size() != 4)
        return;

    Vec2f offset(0.01f, 0.01f);

    const Vec2f& l00 = m_Line0->getPositionStart();
    const Vec2f& l01 = m_Line0->getPositionEnd();
    const Vec2f& l10 = m_Line1->getPositionStart();
    const Vec2f& l11 = m_Line1->getPositionEnd();

    m_LineTrans0->setPositionStart(l00 + offset);
    m_LineTrans0->setPositionEnd(l01 + offset);
    m_LineTrans1->setPositionStart(l10 + offset);
    m_LineTrans1->setPositionEnd(l11 + offset);
}
