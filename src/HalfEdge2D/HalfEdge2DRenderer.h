#ifndef _HALFEDGE_RENDERER_H_
#define _HALFEDGE_RENDERER_H_

#include "Base/Vector.h"

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
    void updateMatrices(ViewPort* const vp);
    QPointF transform(const QPointF& point);

private:
    QWidget* m_Widget;
    Scene* m_Scene;

    Mat3f m_transMat;
};

#endif //_HALFEDGE_WIDGET_H_
