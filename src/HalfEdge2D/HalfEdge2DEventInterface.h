#ifndef _HALFEDGE_EVENTINTERFACE_H_
#define _HALFEDGE_EVENTINTERFACE_H_

class QMouseEvent;
class QResizeEvent;
class QWheelEvent;
class QPaintEvent;

class HalfEdge2DEventHandler;

class HalfEdge2DEventInterface
{
    friend class HalfEdge2DEventHandler;

public:
    HalfEdge2DEventInterface() {}
    virtual ~HalfEdge2DEventInterface() {}

public:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) = 0;
    virtual bool handleMousePressEvent(QMouseEvent* const event) = 0;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) = 0;
    virtual bool handleResizeEvent(QResizeEvent* const event) = 0;
    virtual bool handleWheelEvent(QWheelEvent* const event) = 0;
    
    virtual void handlePaintEvent(QPaintEvent* const event) {};
};

#endif //_HALFEDGE_EVENTHANDLER_H_
