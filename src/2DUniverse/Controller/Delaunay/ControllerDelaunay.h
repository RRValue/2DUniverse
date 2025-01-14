#ifndef _CONTROLLER_CONTROLLERDELAUNAY_H_
#define _CONTROLLER_CONTROLLERDELAUNAY_H_

#include "2DUniverse/Controller/Controller.h"

#include "2DUniverse/Base/Vector.h"

#include <vector>
#include <array>

class Point;
class Circle;
class Line;

class ControllerDelaunay : public Controller
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
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
    Point* const getPointAtPos(const Vec2d& pos) const;
    void updateCircumCircles();
    void updateCircumCircle(Circle* const circle, const std::array<Point* const, 3>& points);
    bool isPointInCircle(const std::array<Point* const, 3>& points, Point* const point);
    void updateLines();

private:
    bool m_MovePoint;
    
    Point* m_CurrentPoint;
    Vec2d m_CurrentHitDistance;

    std::vector<Point*> m_Points;
    std::vector<Circle*> m_CircumCircles;
    std::vector<Line*> m_Lines;
};

#endif //_CONTROLLER_CONTROLLERDELAUNAY_H_
