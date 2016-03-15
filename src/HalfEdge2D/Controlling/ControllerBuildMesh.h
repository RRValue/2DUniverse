#ifndef _CONTROLLING_CONTROLLERBUILDMESH_H_
#define _CONTROLLING_CONTROLLERBUILDMESH_H_

#include "HalfEdge2D/Controlling/Controller.h"

class Scene;
class Camera;

class ControllerBuildMesh : public Controller
{
public:
    ControllerBuildMesh();
    virtual ~ControllerBuildMesh();

    void setScene(Scene* const scene);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    bool m_MovePoint;

    Scene* m_Scene;
    
    int m_CurrentIdx;
    QPointF m_CurrentHitDistance;
};

#endif //_CONTROLLING_CONTROLLERBUILDMESH_H_
