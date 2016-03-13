#ifndef _HALFEDGE_CONTROLLER_H_
#define _HALFEDGE_CONTROLLER_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtCore/QPointF>

class HESMesh;
class RenderTarget;
class Scene;

class HalfEdge2DController : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DController() = delete;
    HalfEdge2DController(RenderTarget* const target);
    virtual ~HalfEdge2DController();

    void setScene(Scene* const scene);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    Scene* m_Scene;

    bool m_MovePoint;

    HESMesh* m_Mesh;
    RenderTarget* m_Target;

    int m_CurrentIdx;
    QPointF m_CurrentHitDistance;
};

#endif //_HALFEDGE_CONTROLLER_H_
