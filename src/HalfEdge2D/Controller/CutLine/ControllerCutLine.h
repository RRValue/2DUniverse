#ifndef _CONTROLLER_CONTROLLERCUTLINE_H_
#define _CONTROLLER_CONTROLLERCUTLINE_H_

#include "HalfEdge2D/Controller/Controller.h"

#include <HalfEdge2D/Base/Vector.h>

#include <vector>
#include <array>

class Point;
class Line;

class ControllerCutLine : public Controller
{
public:
    ControllerCutLine();
    virtual ~ControllerCutLine();

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
    Point* const getPointAtPos(const Vec2f& pos, size_t* const idx) const;
    void cut();

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2f m_CurrentHitDistance;

    std::vector<Point* const> m_Points;
    Line* m_Line0;
    Line* m_Line1;

    Point* m_CutPoint;
};

#endif //_CONTROLLER_CONTROLLERCUTLINE_H_
