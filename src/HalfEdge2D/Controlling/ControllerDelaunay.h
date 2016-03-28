#ifndef _CONTROLLING_CONTROLLERDELAUNAY_H_
#define _CONTROLLING_CONTROLLERDELAUNAY_H_

#include "HalfEdge2D/Controlling/Controller.h"

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
    int getPointAtPos(const QPointF& pos) const;

private:
    bool m_MovePoint;
    
    int m_CurrentIdx;
    QPointF m_CurrentHitDistance;

    HESMesh* m_Mesh;
};

#endif //_CONTROLLING_CONTROLLERDELAUNAY_H_
