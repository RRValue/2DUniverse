#ifndef _CONTROLLING_CONTROLLERDELAUNAY_H_
#define _CONTROLLING_CONTROLLERDELAUNAY_H_

#include "HalfEdge2D/Controlling/Controller.h"

#include <HalfEdge2D/Base/Vector.h>

#include <vector>

typedef std::vector<Vec2f, Eigen::aligned_allocator<Vec2f>> PointVector;

class HESMesh;

class ControllerDelaunay : public Controller
{
public:
    ControllerDelaunay();
    virtual ~ControllerDelaunay();

    void setMesh(HESMesh* const mesh);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    int getPointAtPos(const Vec2f& pos) const;

private:
    bool m_MovePoint;
    
    int m_CurrentIdx;
    Vec2f m_CurrentHitDistance;

    HESMesh* m_Mesh;

    PointVector m_Points;
};

#endif //_CONTROLLING_CONTROLLERDELAUNAY_H_
