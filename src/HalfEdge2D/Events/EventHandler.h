#ifndef _EVENTS_EVENTHANDLER_H_
#define _EVENTS_EVENTHANDLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include <QtCore/QPoint>

#include <vector>

class Navigator;
class Controller;
class Renderer;
class ViewPort;
class QPaintTarget;

class EventHandler : public EventInterface
{
public:
    EventHandler() = delete;
    EventHandler(QPaintTarget* const paintTarget);
    ~EventHandler();

    void setNavigator(EventInterface* const navigator);
    void setController(EventInterface* const controller);
    void setRenderer(Renderer* const renderer);
    ViewPort* const getActiveViewPort();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

    virtual void handlePaintEvent(QPaintEvent* const event) final override;

private:
    void setActiveRenderTarget();
    void setActiveViewport(const QPoint& point);
    void setActiveCamera();

private:
    QPaintTarget* m_PaintTarget;

    EventInterface* m_Navigator;
    EventInterface* m_Controller;

    ViewPort* m_ActiveViewPort;
};

#endif //_EVENTS_EVENTHANDLER_H_
