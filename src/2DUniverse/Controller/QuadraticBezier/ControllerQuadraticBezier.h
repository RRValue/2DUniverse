#ifndef _CONTROLLER_CONTROLLERQUADRATICBEZIER_H_
#define _CONTROLLER_CONTROLLERQUADRATICBEZIER_H_

#include "2DUniverse/Controller/Controller.h"

#include "2DUniverse/Base/Vector.h"

#include <vector>
#include <array>

class Point;
class QuadraticBezier;

class ControllerQuadraticBezier : public Controller
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    ControllerQuadraticBezier();
    virtual ~ControllerQuadraticBezier();

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
    Point* const getPointAtPos(const Vec2d& pos, size_t* const idx) const;

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2d m_CurrentHitDistance;

    std::vector<Point*> m_Points;
    QuadraticBezier* m_Bezier;
};

#endif //_CONTROLLER_CONTROLLERQUADRATICBEZIER_H_
