#ifndef _CONTROLLER_CONTROLLERCUTCUBICBEZIER_H_
#define _CONTROLLER_CONTROLLERCUTCUBICBEZIER_H_

#include "HalfEdge2D/Controller/Controller.h"

#include <HalfEdge2D/Base/Vector.h>

#include <vector>
#include <array>

class Point;
class Line;
class CubicBezier;

class ControllerCutCubicBezier : public Controller
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    ControllerCutCubicBezier();
    virtual ~ControllerCutCubicBezier();

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
    void cut();

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2d m_CurrentHitDistance;

    std::vector<Point*> m_Points;
    Line* m_Line;
    CubicBezier* m_Bezier;

    Point* m_CutPoint0;
    Point* m_CutPoint1;
    Point* m_CutPoint2;
};

#endif //_CONTROLLER_CONTROLLERCUTCUBICBEZIER_H_
