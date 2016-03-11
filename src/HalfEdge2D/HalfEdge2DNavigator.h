#ifndef _HALFEDGE_NAVIGATOR_H_
#define _HALFEDGE_NAVIGATOR_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtCore/QPointF>
#include <QtCore/QSize>

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
    bool inWidget(const QPoint& point);
    QPoint keepInWidget(const QPoint& point);

    void updateRelativeBases();

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

    bool m_Navigatin;

    // widget size
    QSize m_CurrentSize;
    float m_AspectRatio;

    // camera
    QPointF m_OrthoSize;
    QPointF m_CameraPosition;

    // scene manipulation
    float m_ZoomFactor;
    QPointF m_CamMoveInitMousePos;
    QPointF m_CamMoveInitCamPos;
};

#endif //_HALFEDGE_NAVIGATOR_H_
