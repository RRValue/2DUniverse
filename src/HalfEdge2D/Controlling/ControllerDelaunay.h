#ifndef _CONTROLLING_CONTROLLERDELAUNAY_H_
#define _CONTROLLING_CONTROLLERDELAUNAY_H_

#include "HalfEdge2D/Controlling/Controller.h"

#include <HalfEdge2D/Base/Vector.h>

#include <set>

class Point;
class Circle;

class ControllerDelaunay : public Controller
{
public:
    ControllerDelaunay();
    virtual ~ControllerDelaunay();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    Point* const getPointAtPos(const Vec2f& pos) const;
    void updateCircumCircle();

private:
    bool m_MovePoint;
    
    Point* m_CurrentPoint;
    Vec2f m_CurrentHitDistance;

    std::set<Point* const> m_Points;

    Circle* m_CircumCircle;
};

#endif //_CONTROLLING_CONTROLLERDELAUNAY_H_
