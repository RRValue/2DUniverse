#ifndef _CONTROLLING_CONTROLLERBUILDMESH_H_
#define _CONTROLLING_CONTROLLERBUILDMESH_H_

#include "HalfEdge2D/Controlling/Controller.h"

class HESMesh;

class ControllerBuildMesh : public Controller
{
public:
    ControllerBuildMesh();
    virtual ~ControllerBuildMesh();

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

#endif //_CONTROLLING_CONTROLLERBUILDMESH_H_
