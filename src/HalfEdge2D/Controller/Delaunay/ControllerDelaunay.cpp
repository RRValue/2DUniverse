#include "HalfEdge2D/Controller/Delaunay/ControllerDelaunay.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Circle.h"
#include "HalfEdge2D/Renderables/Line.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"

#include <QtGui/QMouseEvent>

ControllerDelaunay::ControllerDelaunay()
{
    m_Name = "ControllerDelaunay";
}

ControllerDelaunay::~ControllerDelaunay()
{

}

void ControllerDelaunay::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;
}

void ControllerDelaunay::activate()
{

}

void ControllerDelaunay::deactivate()
{

}

bool ControllerDelaunay::handleMouseMoveEvent(QMouseEvent* const event)
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

    updateLines();
    updateCircumCircles();

    m_Renderer->render();

    return true;
}

bool ControllerDelaunay::handleMousePressEvent(QMouseEvent* const event)
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

    m_CurrentPoint = getPointAtPos(invTrans(p_f));

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

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);
    }

    updateLines();
    updateCircumCircles();

    m_Renderer->render();
    
    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

void ControllerDelaunay::updateLines()
{
    if(m_Lines.empty())
    {
        m_Lines.push_back(new Line());
        m_Lines.push_back(new Line());
        m_Lines.push_back(new Line());
        m_Lines.push_back(new Line());
        m_Lines.push_back(new Line());

        for(const auto& l : m_Lines)
        {
            l->setVisible(false);
            
            m_Scene->addLine(l);
        }
    }

    if(m_Points.size() >= 2)
    {
        m_Lines[0]->setPoint(0, m_Points[0]->getPosition());
        m_Lines[0]->setPoint(1, m_Points[1]->getPosition());
        m_Lines[0]->setVisible(true);
    }

    if(m_Points.size() >= 3)
    {
        m_Lines[1]->setPoint(0, m_Points[1]->getPosition());
        m_Lines[1]->setPoint(1, m_Points[2]->getPosition());
        m_Lines[1]->setVisible(true);
    }

    if(m_Points.size() >= 4)
    {
        m_Lines[2]->setPoint(0, m_Points[2]->getPosition());
        m_Lines[2]->setPoint(1, m_Points[3]->getPosition());
        m_Lines[2]->setVisible(true);

        m_Lines[3]->setPoint(0, m_Points[3]->getPosition());
        m_Lines[3]->setPoint(1, m_Points[0]->getPosition());
        m_Lines[3]->setVisible(true);

        if(isPointInCircle({m_Points[0], m_Points[1], m_Points[2]}, m_Points[3]))
        {
            m_Lines[4]->setPoint(0, m_Points[0]->getPosition());
            m_Lines[4]->setPoint(1, m_Points[2]->getPosition());
        }
        else
        {
            m_Lines[4]->setPoint(0, m_Points[1]->getPosition());
            m_Lines[4]->setPoint(1, m_Points[3]->getPosition());
        }

        m_Lines[4]->setVisible(true);
        m_Lines[4]->setColour(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    }
}

bool ControllerDelaunay::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerDelaunay::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerDelaunay::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerDelaunay::getPointAtPos(const Vec2f& pos) const
{
    if(m_Points.empty())
        return nullptr;

    // TODO point size from renderable point
    for(const auto& p : m_Points)
        if((p->getPosition() - pos).norm() < p->getSize())
            return p;

    return nullptr;
}

bool ControllerDelaunay::isPointInCircle(const std::array<Point* const, 3>& points, Point* const point)
{
    const Vec2f& p0 = m_Points[0]->getPosition();
    const Vec2f& p1 = m_Points[1]->getPosition();
    const Vec2f& p2 = m_Points[2]->getPosition();
    const Vec2f& p3 = point->getPosition();

    Vec2f p02(p0[0] * p0[0], p0[1] * p0[1]);
    Vec2f p12(p1[0] * p1[0], p1[1] * p1[1]);
    Vec2f p22(p2[0] * p2[0], p2[1] * p2[1]);
    Vec2f p32(p3[0] * p3[0], p3[1] * p3[1]);

    Mat3f m;
    m <<
        p0[0] - p3[0], p0[1] - p3[1], p02[0] - p32[0] + p02[1] - p32[1],
        p1[0] - p3[0], p1[1] - p3[1], p12[0] - p32[0] + p12[1] - p32[1],
        p2[0] - p3[0], p2[1] - p3[1], p22[0] - p32[0] + p22[1] - p32[1];

    return m.determinant() > 0.0f;
}

void ControllerDelaunay::updateCircumCircles()
{
    if(m_Points.size() < 3)
        return;

    if(m_CircumCircles.empty())
    {
        m_CircumCircles.push_back(new Circle());
        m_CircumCircles.push_back(new Circle());

        m_CircumCircles[0]->setVisible(false);
        m_CircumCircles[1]->setVisible(false);

        m_CircumCircles[0]->setColour(Vec4f(0.75f, 0.75f, 0.75f, 1.0f));
        m_CircumCircles[1]->setColour(Vec4f(0.75f, 0.75f, 0.75f, 1.0f));

        m_Scene->addCircle(m_CircumCircles[0]);
        m_Scene->addCircle(m_CircumCircles[1]);
    }

    if(m_Points.size() >= 3)
        updateCircumCircle(m_CircumCircles[0], {m_Points[0], m_Points[1], m_Points[2]});

    if(m_Points.size() >= 4)
        updateCircumCircle(m_CircumCircles[1], {m_Points[1], m_Points[2], m_Points[3]});
}

void ControllerDelaunay::updateCircumCircle(Circle* const circle, const std::array<Point* const, 3>& points)
{
    Line l01;
    Line l12;

    l01.setPoint(0, points[0]->getPosition());
    l01.setPoint(1, points[1]->getPosition());
    l12.setPoint(0, points[1]->getPosition());
    l12.setPoint(1, points[2]->getPosition());

    float length = 1000.0f * (l01.getLength() + l12.getLength());

    Vec2f bn0(l01.getNormal()[1], -l01.getNormal()[0]);
    Vec2f bn1(l12.getNormal()[1], -l12.getNormal()[0]);

    Vec2f sp0 = l01.pointAt(0.5f);
    Vec2f sp1 = l12.pointAt(0.5f);

    Vec2f bs0s = sp0 - bn0 * length;
    Vec2f bs0e = sp0 + bn0 * length;
    Vec2f bs1s = sp1 - bn1 * length;
    Vec2f bs1e = sp1 + bn1 * length;

    Line bisect0;
    Line bisect1;

    bisect0.setPoint(0, bs0s);
    bisect0.setPoint(1, bs0e);
    bisect1.setPoint(0, bs1s);
    bisect1.setPoint(1, bs1e);

    Vec2fVec center = bisect0.intersect(bisect1);

    circle->setVisible(!center.empty());

    float radius = (points[0]->getPosition() - center[0]).norm();

    circle->setPosition(center[0]);
    circle->setRadius(radius);
}
