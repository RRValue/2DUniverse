#ifndef _HALFEDGE_NAVIGATOR_H_
#define _HALFEDGE_NAVIGATOR_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QSize>
#include <QtCore/QPointF>

class RenderTarget;

class Camera;

class HalfEdge2DNavigator : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DNavigator() = delete;
    HalfEdge2DNavigator(RenderTarget* const target);
    virtual ~HalfEdge2DNavigator();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    int getHitPoint(const QPointF& pos);

    void zoom(const int& step, const QPoint& pos_px);

    bool inViewPort(const QPoint& point) const;
    QPoint keepInViewPort(const QPoint& point) const;

    void updateTransMatrix();
    QPointF trans(const QPointF& point);
    QPointF invTrans(const QPointF& point);

private:
    RenderTarget* m_RenderTarget;

    Camera* m_ActiveCamera;

    bool m_Navigatin;

    // scene manipulation
    float m_ZoomFactor;
    QPointF m_CamMoveInitMousePos;
    QPointF m_CamMoveInitCamPos;

    // scene matrices
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;
};

#endif //_HALFEDGE_NAVIGATOR_H_
