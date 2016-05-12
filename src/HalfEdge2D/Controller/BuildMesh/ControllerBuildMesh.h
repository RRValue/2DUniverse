#ifndef _CONTROLLER_CONTROLLERBUILDMESH_H_
#define _CONTROLLER_CONTROLLERBUILDMESH_H_

#include "HalfEdge2D/Controller/Controller.h"

class HESMesh;

class ControllerBuildMesh : public Controller
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    ControllerBuildMesh();
    virtual ~ControllerBuildMesh();

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
    int getPointAtPos(const Vec2d& pos) const;

private:
    bool m_MovePoint;
    
    int m_CurrentIdx;
    Vec2d m_CurrentHitDistance;

    HESMesh* m_Mesh;
};

#endif //_CONTROLLER_CONTROLLERBUILDMESH_H_
