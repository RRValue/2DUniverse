#ifndef _EVENTS_EVENTINTERFACE_H_
#define _EVENTS_EVENTINTERFACE_H_

class QMouseEvent;
class QResizeEvent;
class QWheelEvent;
class QPaintEvent;

class EventHandler;
class ViewPort;
class RenderTarget;
class Renderer;
class Camera;

class EventInterface
{
    friend class EventHandler;

public:
    EventInterface() {}
    virtual ~EventInterface() {}

public:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) = 0;
    virtual bool handleMousePressEvent(QMouseEvent* const event) = 0;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) = 0;
    virtual bool handleResizeEvent(QResizeEvent* const event) = 0;
    virtual bool handleWheelEvent(QWheelEvent* const event) = 0;
    
    virtual void handlePaintEvent(QPaintEvent* const event) {};

protected:
    Renderer* m_Renderer;
    RenderTarget* m_RenderTarget;
    ViewPort* m_ActiveViewPort;
    Camera* m_ActiveCamera;
};

#endif //_EVENTS_EVENTINTERFACE_H_
