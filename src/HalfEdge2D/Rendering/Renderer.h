#ifndef _RENDERING_RENDERER_H_
#define _RENDERING_RENDERER_H_

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QPointF>

#include <set>

class QPaintEvent;
class QPainter;

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
    void render(QPaintEvent* const event, QPaintTarget* const paintTarget);
    void render(QPaintTarget* const paintTarget);

    void setScene(Scene* const scene);
    void addPaintTarget(QPaintTarget* const paintTarget);
    void removePaintTarget(QPaintTarget* const paintTarget);

private:
    void updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp);
    QPointF trans(const QPointF& point);
    QPointF transToDevice(const QPointF& point);

    void paint(QPaintTarget* const paintTarget);

private:
    void renderMesh(QPainter* const painter, Mesh* const mesh);

private:
    Scene* m_Scene;

    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;

    std::set<QPaintTarget* const> m_PaintTargets;

    const float m_PointSize;
};

#endif //_RENDERING_RENDERER_H_
