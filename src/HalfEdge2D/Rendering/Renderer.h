#ifndef _RENDERING_RENDERER_H_
#define _RENDERING_RENDERER_H_

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QPointF>

#include <set>

class QPaintEvent;
class QPainter;

class QWidgetTarget;
class Scene;
class RenderTarget;
class ViewPort;
class Mesh;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void render();
    void render(QPaintEvent* const event, QWidgetTarget* const widgetTarget);
    void render(QWidgetTarget* const widgetTarget);

    void setScene(Scene* const scene);
    void addWidgetTarget(QWidgetTarget* const widgetTarget);
    void removeWidgetTarget(QWidgetTarget* const widgetTarget);

private:
    void updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp);
    QPointF trans(const QPointF& point);
    QPointF transToDevice(const QPointF& point);

    void paint(QWidgetTarget* const widgetTarget);

private:
    void renderMesh(QPainter* const painter, Mesh* const mesh);

private:
    Scene* m_Scene;

    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;

    std::set<QWidgetTarget* const> m_WidgetTargets;

    const float m_PointSize;
};

#endif //_RENDERING_RENDERER_H_
