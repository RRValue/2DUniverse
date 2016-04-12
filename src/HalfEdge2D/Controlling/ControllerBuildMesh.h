#ifndef _CONTROLLING_CONTROLLERBUILDMESH_H_
#define _CONTROLLING_CONTROLLERBUILDMESH_H_

#include "HalfEdge2D/Controlling/Controller.h"

class ControllerBuildMesh : public Controller
{
public:
    ControllerBuildMesh();
    virtual ~ControllerBuildMesh();

protected:
    virtual void activate() final override;
    virtual void deactivate() final override;

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
};

#endif //_CONTROLLING_CONTROLLERBUILDMESH_H_
