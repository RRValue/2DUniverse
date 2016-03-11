#ifndef _HALFEDGE_NAVIGATOR_H_
#define _HALFEDGE_NAVIGATOR_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

class HalfEdge2DWidget;

class HalfEdge2DNavigator : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DNavigator() = delete;
    HalfEdge2DNavigator(HalfEdge2DWidget* const widget);
    virtual ~HalfEdge2DNavigator();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    HalfEdge2DWidget* m_Widget;
};

#endif //_HALFEDGE_NAVIGATOR_H_
