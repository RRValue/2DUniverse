#ifndef _EVENTS_EVENTHANDLER_H_
#define _EVENTS_EVENTHANDLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include <QtCore/QPoint>

#include <vector>

class Navigator;
class Controller;
class Renderer;
class ViewPort;

class EventHandler : public EventInterface
{
public:
    EventHandler() = delete;
    EventHandler(RenderTarget* const renderTarget);
    ~EventHandler();

    void addEventInterface(EventInterface* const eventInterface);
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

private:
    std::vector<EventInterface* const> m_EventInterfaces;
    Renderer* m_Renderer;

    ViewPort* m_ActiveViewPort;
};

#endif //_EVENTS_EVENTHANDLER_H_
