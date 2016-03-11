#ifndef _HALFEDGE_CONTROLLER_H_
#define _HALFEDGE_CONTROLLER_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

class HESMesh;
class HalfEdge2DWidget;

class HalfEdge2DController : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DController() = delete;
    HalfEdge2DController(HalfEdge2DWidget* const widget);
    virtual ~HalfEdge2DController();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    HESMesh* m_Mesh;
    HalfEdge2DWidget* m_Widget;
};

#endif //_HALFEDGE_CONTROLLER_H_
