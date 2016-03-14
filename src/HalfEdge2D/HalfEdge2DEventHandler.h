#ifndef _HALFEDGE_EVENTHANDLER_H_
#define _HALFEDGE_EVENTHANDLER_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtCore/QPoint>

#include <vector>

class HalfEdge2DNavigator;
class HalfEdge2DController;
class HalfEdge2DRenderer;
class RenderTarget;
class ViewPort;

class HalfEdge2DEventHandler : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DEventHandler() = delete;
    HalfEdge2DEventHandler(RenderTarget* const renderTarget);
    ~HalfEdge2DEventHandler();

    void addEventInterface(HalfEdge2DEventInterface* const eventInterface);
    void setRenderer(HalfEdge2DRenderer* const renderer);
    ViewPort* const getActiveViewPort();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

    virtual void handlePaintEvent(QPaintEvent* const event) final override;

private:
    void setActiveViewport(const QPoint& point);

private:
    RenderTarget* const m_RenderTarget;
    std::vector<HalfEdge2DEventInterface* const> m_EventInterfaces;
    HalfEdge2DRenderer* m_Renderer;

    ViewPort* m_ActiveViewPort;
};

#endif //_HALFEDGE_EVENTHANDLER_H_
