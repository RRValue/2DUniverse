#ifndef _RENDERING_RENDERER_H_
#define _RENDERING_RENDERER_H_

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QPointF>
#include <QtCore/QRectF>

#include <set>

class QPaintEvent;
class QPainter;
class QPaintDevice;

class QWidgetTarget;
class QPaintTarget;
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
    void render(QPaintTarget* const paintTarget);

    void setScene(Scene* const scene);
    void addWidgetTarget(QWidgetTarget* const widgetTarget);
    void removeWidgetTarget(QWidgetTarget* const widgetTarget);

    void setRenderViewport(const bool& render);
    void setRenderCoordianteAxis(const bool& render);
    void setRenderVertices(const bool& render);
    void setRenderTriangles(const bool& render);
    void setRenderTrianglesEdges(const bool& render);

private:
    void updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp);
    QPointF trans(const QPointF& point);
    QPointF transToDevice(const QPointF& point);

    void paint(QPaintDevice* const paintDevice, RenderTarget* const renderTarget);

private:
    void renderScene(QPainter* const painter, Scene* const scene);
    void renderMesh(QPainter* const painter, Mesh* const mesh);
    void renderViewport(QPainter* const painter, const QRectF& vp);
    void renderCoordinateCross(QPainter* const painter);

private:
    bool m_RenderVertices;
    bool m_RenderTriangles;
    bool m_RenderTrianglesEdges;
    bool m_RenderCoordinateAxis;
    bool m_RenderViewport;

    Scene* m_Scene;

    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;

    std::set<QWidgetTarget* const> m_WidgetTargets;

    const float m_PointSize;
};

#endif //_RENDERING_RENDERER_H_
