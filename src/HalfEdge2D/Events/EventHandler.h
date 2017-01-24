#ifndef _EVENTS_EVENTHANDLER_H_
#define _EVENTS_EVENTHANDLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include <QtCore/QPoint>

#include <set>

class Navigator;
class Controller;
class Renderer;
class QWidgetTarget;

class EventHandler : public EventInterface
{
public:
    EventHandler() = delete;
    EventHandler(QWidgetTarget* const widgetTarget);
    ~EventHandler();

    void setNavigator(EventInterface* const navigator);
    void addController(Controller* const controller);
    void removeController(Controller* const controller);
    void setRenderer(Renderer* const renderer);
    void setActiveController(Controller* const controller);
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

    void viewportContentChanged();

private:
    QWidgetTarget* m_WidgetTarget;

    EventInterface* m_Navigator;
    std::set<Controller*> m_Controller;
    Controller* m_ActiveController;
};

#endif //_EVENTS_EVENTHANDLER_H_
