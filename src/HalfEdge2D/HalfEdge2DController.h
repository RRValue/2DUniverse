#ifndef _HALFEDGE_CONTROLLER_H_
#define _HALFEDGE_CONTROLLER_H_

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtCore/QVector>
#include <QtCore/QPointF>

class HESMesh;
class HalfEdge2DWidget;

class HalfEdge2DController : public HalfEdge2DEventInterface
{
public:
    HalfEdge2DController() = delete;
    HalfEdge2DController(HalfEdge2DWidget* const widget);
    virtual ~HalfEdge2DController();

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    int getHitPoint(const QPointF& pos);

private:
    bool m_MovePoint;

    HESMesh* m_Mesh;
    HalfEdge2DWidget* m_Widget;

    int m_CurrentIdx;
    QVector<QPointF> m_Points;
    QPointF m_CurrentHitDistance;

    float m_PointSizePx;
};

#endif //_HALFEDGE_CONTROLLER_H_
