#ifndef _CONTROLLER_CONTROLLERSPLINE_H_
#define _CONTROLLER_CONTROLLERSPLINE_H_

#include "HalfEdge2D/Controller/Controller.h"

#include <HalfEdge2D/Base/Vector.h>

#include <vector>

class Point;
class Spline;

class ControllerSpline : public Controller
{
public:
    ControllerSpline();
    virtual ~ControllerSpline();

protected:
    virtual void activate() final override;
    virtual void deactivate() final override;

    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    Point* const getPointAtPos(const Vec2f& pos, size_t* const idx) const;

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2f m_CurrentHitDistance;

    std::vector<Point* const> m_Points;
    Spline* m_Spline;
};

#endif //_CONTROLLER_CONTROLLERSPLINE_H_
