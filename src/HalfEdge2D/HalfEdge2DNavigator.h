#ifndef _HALFEDGE_NAVIGATOR_H_
#define _HALFEDGE_NAVIGATOR_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtCore/QSize>
#include <QtCore/QPointF>

class HalfEdge2DWidget;

class Scene;
class Camera;

class HalfEdge2DNavigator : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DNavigator() = delete;
    HalfEdge2DNavigator(HalfEdge2DWidget* const widget);
    virtual ~HalfEdge2DNavigator();

    void setScene(Scene* const scene);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    void updateCameraInformation();

private:
    bool inCanvas(const QPoint& point);
    QPoint keepInCanvas(const QPoint& point);

    QPointF toView(const QPointF& p);
    QPointF fromView(const QPointF& p);

    QPointF toDeviceCoords(const QPointF& point);
    QPointF fromDeviceCoords(const QPointF& point);

    QPointF toWidgetCoords(const QPointF& point);
    QPointF fromWidgetCoords(const QPointF& point);

    QPointF transform(const QPointF& point);
    QPointF invTransform(const QPointF& point);

    int getHitPoint(const QPointF& pos);

    void zoom(const int& step, const QPoint& pos_px);

private:
    HalfEdge2DWidget* m_Widget;

    Scene* m_Scene;
    Camera* m_ActiveCamera;

    bool m_Navigatin;

    // scene manipulation
    float m_ZoomFactor;
    QPointF m_CamMoveInitMousePos;
    QPointF m_CamMoveInitCamPos;
};

#endif //_HALFEDGE_NAVIGATOR_H_
