#ifndef _CONTROLLER_CONTROLLERDELAUNAY_H_
#define _CONTROLLER_CONTROLLERDELAUNAY_H_

#include "HalfEdge2D/Controller/Controller.h"

#include <HalfEdge2D/Base/Vector.h>

#include <vector>
#include <array>

class Point;
class Circle;
class Line;

class ControllerDelaunay : public Controller
{
public:
    ControllerDelaunay();
    virtual ~ControllerDelaunay();

protected:
    virtual void init() final override;
    virtual void activate() final override;
    virtual void deactivate() final override;

    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    Point* const getPointAtPos(const Vec2f& pos) const;
    void updateCircumCircles();
    void updateCircumCircle(Circle* const circle, const std::array<Point* const, 3>& points);
    bool isPointInCircle(const std::array<Point* const, 3>& points, Point* const point);
    void updateLines();

private:
    bool m_MovePoint;
    
    Point* m_CurrentPoint;
    Vec2f m_CurrentHitDistance;

    std::vector<Point* const> m_Points;
    std::vector<Circle* const> m_CircumCircles;
    std::vector<Line* const> m_Lines;
};

#endif //_CONTROLLER_CONTROLLERDELAUNAY_H_
