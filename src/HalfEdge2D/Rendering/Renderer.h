#ifndef _RENDERING_RENDERER_H_
#define _RENDERING_RENDERER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Base/NGradeBezier.h"

#include <QtCore/QPointF>
#include <QtCore/QRectF>

#include <set>
#include <vector>

class QPaintEvent;
class QPainter;
class QPaintDevice;

class QWidgetTarget;
class QPaintTarget;
class Scene;
class Point;
class Circle;
class Line;
class QuadraticBezier;
class CubicBezier;
class Spline;
class RenderTarget;
class ViewPort;
class Mesh;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

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
    void setSmoothRendering(const bool& smooth);

private:
    void updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp);
    QPointF trans(const QPointF& point);
    QPointF transToDevice(const QPointF& point);

    void paint(QPaintDevice* const paintDevice, RenderTarget* const renderTarget);

    float getPixelSize(const float& value);

private:
    void renderScene(QPainter* const painter, Scene* const scene);
    
    void renderMeshes(QPainter* const painter, const std::set<Mesh* const>& meshes);
    void renderMesh(QPainter* const painter, Mesh* const meshes);
    
    void renderPoints(QPainter* const painter, const std::set<Point* const>& points);
    void renderPoint(QPainter* const painter, Point* point);

    void renderCircles(QPainter* const painter, const std::set<Circle* const>& circles);
    void renderCircle(QPainter* const painter, Circle* const circle);

    void renderLines(QPainter* const painter, const std::set<Line* const>& lines);
    void renderLine(QPainter* const painter, Line* const line);
    void renderLine(QPainter* const painter, const Line2F& line);
    
    void renderQuadraticBeziers(QPainter* const painter, const std::set<QuadraticBezier* const>& beziers);
    void renderQuadraticBezier(QPainter* const painter, QuadraticBezier* const bezier);
    void renderQuadraticBezier(QPainter* const painter, const QBezier2F& bezier);

    void renderCubicBeziers(QPainter* const painter, const std::set<CubicBezier* const>& beziers);
    void renderCubicBezier(QPainter* const painter, CubicBezier* const bezier);
    void renderCubicBezier(QPainter* const painter, const CBezier2F& bezier);

    void renderSplines(QPainter* const painter, const std::set<Spline* const>& splines);
    void renderSpline(QPainter* const painter, Spline* const spline);

    void renderViewport(QPainter* const painter, const QRectF& vp);
    void renderCoordinateCross(QPainter* const painter);

private:
    bool m_RenderVertices;
    bool m_RenderTriangles;
    bool m_RenderTrianglesEdges;
    bool m_RenderCoordinateAxis;
    bool m_RenderViewport;
    bool m_SmoothRendering;

    Scene* m_Scene;

    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;

    Vec4f m_ClearColor;

    std::set<QWidgetTarget* const> m_WidgetTargets;

    const float m_PointSize;
};

#endif //_RENDERING_RENDERER_H_
