#ifndef _HALFEDGE_RENDERER_H_
#define _HALFEDGE_RENDERER_H_

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QPointF>

class QPaintEvent;

class QWidget;
class Scene;
class RenderTarget;
class ViewPort;

class HalfEdge2DRenderer
{
public:
    HalfEdge2DRenderer();
    ~HalfEdge2DRenderer();

    void render(QPaintEvent* const event, RenderTarget* const renderTarget);

    void setWidget(QWidget* const widget);
    void setScene(Scene* const scene);

private:
    void updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp);
    QPointF trans(const QPointF& point);
    QPointF transToDevice(const QPointF& point);

private:
    QWidget* m_Widget;
    Scene* m_Scene;

    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;
};

#endif //_HALFEDGE_WIDGET_H_
