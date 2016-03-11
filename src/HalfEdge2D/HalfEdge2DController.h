#ifndef _HALFEDGE_CONTROLLER_H_
#define _HALFEDGE_CONTROLLER_H_

class QMouseEvent;
class QResizeEvent;
class QPaintEvent;
class QWheelEvent;

class HalfEdge2DController
{
public:
    HalfEdge2DController();
    ~HalfEdge2DController();

public:
    bool onMouseMoveEvent(QMouseEvent* event);
    bool onMousePressEvent(QMouseEvent* event);
    bool onMouseReleaseEvent(QMouseEvent* event);
    bool onResizeEvent(QResizeEvent* event);
    bool onPaintEvent(QPaintEvent* event);
    bool onWheelEvent(QWheelEvent* event);
};

#endif //_HALFEDGE_CONTROLLER_H_
