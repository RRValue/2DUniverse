#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/QWidgetTarget.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"
#include "HalfEdge2D/Rendering/RenderWidget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include "HalfEdge2D/Mesh/Mesh.h"
#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Circle.h"
#include "HalfEdge2D/Renderables/Line.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"

#include <QtGui/QPainter>

#include <QtWidgets/QWidget>

Renderer::Renderer() : m_PointSize(0.005f)
{
    m_RenderVertices = true;
    m_RenderTriangles = true;
    m_RenderTrianglesEdges = true;
    m_RenderCoordinateAxis = true;
    m_RenderViewport = true;
    m_SmoothRendering = true;
    m_ClearColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
}

Renderer::~Renderer()
{

}

void Renderer::setRenderViewport(const bool& render)
{
    m_RenderViewport = render;
}

void Renderer::setRenderCoordianteAxis(const bool& render)
{
    m_RenderCoordinateAxis = render;
}

void Renderer::setRenderVertices(const bool& render)
{
    m_RenderVertices = render;
}

void Renderer::setRenderTriangles(const bool& render)
{
    m_RenderTriangles = render;
}

void Renderer::setRenderTrianglesEdges(const bool& render)
{
    m_RenderTrianglesEdges = render;
}

void Renderer::setSmoothRendering(const bool& smooth)
{
    m_SmoothRendering = smooth;
}

void Renderer::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    m_Scene = scene;
}

void Renderer::addWidgetTarget(QWidgetTarget* const widgetTarget)
{
    if(widgetTarget == nullptr)
        return;

    if(m_WidgetTargets.find(widgetTarget) != m_WidgetTargets.end())
        return;

    m_WidgetTargets.insert(widgetTarget);
}

void Renderer::removeWidgetTarget(QWidgetTarget* const widgetTarget)
{
    if(widgetTarget == nullptr)
        return;

    if(m_WidgetTargets.find(widgetTarget) == m_WidgetTargets.end())
        return;

    m_WidgetTargets.erase(m_WidgetTargets.find(widgetTarget));
}

void Renderer::render()
{
    for(const auto& pt : m_WidgetTargets)
        pt->render();
}

void Renderer::render(QWidgetTarget* const widgetTarget)
{
    paint(widgetTarget->getPaintDevice(), widgetTarget);
}

void Renderer::render(QPaintTarget* const paintTarget)
{
    paint(paintTarget->getPaintDevice(), paintTarget);
}

void Renderer::render(QPaintEvent* const event, QWidgetTarget* const widgetTarget)
{
    paint(widgetTarget->getPaintDevice(), widgetTarget);
}

void Renderer::paint(QPaintDevice* const paintDevice, RenderTarget* const renderTarget)
{
    QPainter painter(paintDevice);

    if(m_SmoothRendering)
        painter.setRenderHint(QPainter::Antialiasing, true);
    else
        painter.setRenderHint(QPainter::Antialiasing, false);

    for(const auto& vp : renderTarget->getViewPorts())
    {
        updateMatrices(renderTarget, vp);

        // add clipping
        const QRectF& vp_size = vp->getSize();
        const QSizeF& rt_size = renderTarget->getSize();

        QPointF bl = transToDevice(vp_size.bottomLeft());
        QPointF tr = transToDevice(vp_size.topRight());

        QRect clip_region((int)(bl.x() + 0.5f), (int)(bl.y() + 0.5f), (int)(tr.x() - bl.x() + 0.5f), (int)(tr.y() - bl.y() + 0.5f));

        painter.setClipRegion(clip_region);
        painter.setClipping(true);

        painter.fillRect(clip_region, QColor::fromRgbF(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));

        // render scene
        if(m_Scene != nullptr)
            renderScene(&painter, m_Scene);

        renderCoordinateCross(&painter);
        renderViewport(&painter, vp_size);
    }
}

void Renderer::renderViewport(QPainter* const painter, const QRectF& vp)
{
    if(!m_RenderViewport)
        return;

    QPointF p0 = transToDevice(QPointF(vp.left(), vp.bottom()));
    QPointF p1 = transToDevice(QPointF(vp.right(), vp.bottom()));
    QPointF p2 = transToDevice(QPointF(vp.left(), vp.top()));
    QPointF p3 = transToDevice(QPointF(vp.right(), vp.top()));

    painter->setPen(Qt::SolidLine);
    painter->setPen(QPen(Qt::GlobalColor::black));

    painter->drawLine(p0, p1);
    painter->drawLine(p0, p2);

    painter->drawLine(p1, p3);
    painter->drawLine(p2, p3);
}

void Renderer::renderCoordinateCross(QPainter* const painter)
{
    if(!m_RenderCoordinateAxis)
        return;

    painter->setPen(Qt::SolidLine);
    painter->setPen(QPen(Qt::GlobalColor::black));
    painter->drawLine(trans(QPointF(-1.0f, 0.0f)), trans(QPointF(1.0f, 0.0f)));
    painter->drawLine(trans(QPointF(0.0f, -1.0f)), trans(QPointF(0.0f, 1.0f)));
}

void Renderer::renderScene(QPainter* const painter, Scene* const scene)
{
    if(scene->getMesh() != nullptr)
        renderMesh(painter, m_Scene->getMesh());

    renderPoints(painter, scene->getPoints());
    renderCircles(painter, scene->getCircles());
    renderLines(painter, scene->getLines());
    renderCubicBezier(painter, scene->getCubicBeziers());
}

void Renderer::renderPoints(QPainter* const painter, const std::set<Point* const>& points)
{
    for(const auto& p : points)
    {
        QPointF ref = trans(QPointF(0.0f, 0.0f));
        QPointF tar = trans(QPointF(p->getSize(), 0.0f));
        float point_size_px = (tar - ref).manhattanLength();

        const Vec2f& pos = p->getPosition();
        const Vec4f& col = p->getColour();

        QPointF vert_pos(pos[0], pos[1]);

        QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(paint_color));
        painter->drawEllipse(trans(vert_pos), point_size_px, point_size_px);
    }
}

void Renderer::renderCircles(QPainter* const painter, const std::set<Circle* const>& circles)
{
    for(const auto& c : circles)
    {
        if(!c->isVisible())
            continue;

        QPointF ref = trans(QPointF(0.0f, 0.0f));
        QPointF tar = trans(QPointF(c->getRadius(), 0.0f));
        float radius_px = (tar - ref).manhattanLength();

        const Vec2f& pos = c->getPosition();
        const Vec4f& col = c->getColour();

        QPointF circle_pos(pos[0], pos[1]);

        QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

        painter->setPen(QPen(paint_color));
        painter->setBrush(Qt::BrushStyle::NoBrush);
        painter->drawEllipse(trans(circle_pos), radius_px, radius_px);
    }
}

void Renderer::renderLines(QPainter* const painter, const std::set<Line* const>& lines)
{
    for(const auto& l : lines)
    {
        if(!l->isVisible())
            continue;

        QPointF ref = trans(QPointF(0.0f, 0.0f));
        QPointF tar = trans(QPointF(l->getThickness(), 0.0f));
        float thickness_px = (tar - ref).manhattanLength();

        const Vec2f& pos0 = l->getPositionStart();
        const Vec2f& pos1 = l->getPositionEnd();
        const Vec4f& col = l->getColour();

        QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

        painter->setPen(QPen(paint_color));
        painter->setBrush(Qt::BrushStyle::NoBrush);
        painter->drawLine(trans(QPointF(pos0[0], pos0[1])), trans(QPointF(pos1[0], pos1[1])));
    }
}

void Renderer::renderCubicBezier(QPainter* const painter, const std::set<CubicBezier* const>& cubicBeziers)
{

}

void Renderer::renderMesh(QPainter* const painter, Mesh* const mesh)
{
    // get point point size to paint
    QPointF ref = trans(QPointF(0.0f, 0.0f));
    QPointF tar = trans(QPointF(m_PointSize, 0.0f));
    float point_size_px = (tar - ref).manhattanLength();

    const std::vector<Vertex*>& vertices = mesh->getVertices();
    const std::vector<Face*>& faces = mesh->getFaces();

    QColor paint_color;

    if(m_RenderTriangles)
    {
        // paint traingles
        for(const auto& face : faces)
        {
            Vec2f vp0 = vertices[face->getVertIdx(0)]->getPosition();
            Vec2f vp1 = vertices[face->getVertIdx(1)]->getPosition();
            Vec2f vp2 = vertices[face->getVertIdx(2)]->getPosition();

            QPointF p0 = trans(QPointF(vp0.x(), vp0.y()));
            QPointF p1 = trans(QPointF(vp1.x(), vp1.y()));
            QPointF p2 = trans(QPointF(vp2.x(), vp2.y()));

            // fill triangle
            const Vec4f& tris_color = face->getColor();
            paint_color = QColor::fromRgbF(tris_color[0], tris_color[1], tris_color[2], tris_color[3]);

            QPainterPath path;

            path.moveTo(p0);
            path.lineTo(p1);
            path.lineTo(p2);
            path.lineTo(p0);

            painter->setPen(Qt::NoPen);
            painter->fillPath(path, QBrush(paint_color));
        }

        if(m_RenderTrianglesEdges)
        {
            // paint traingles edges
            for(const auto& face : faces)
            {
                Vec2f vp0 = vertices[face->getVertIdx(0)]->getPosition();
                Vec2f vp1 = vertices[face->getVertIdx(1)]->getPosition();
                Vec2f vp2 = vertices[face->getVertIdx(2)]->getPosition();

                QPointF p0 = trans(QPointF(vp0.x(), vp0.y()));
                QPointF p1 = trans(QPointF(vp1.x(), vp1.y()));
                QPointF p2 = trans(QPointF(vp2.x(), vp2.y()));

                painter->setPen(Qt::SolidLine);
                painter->setPen(Qt::black);
                painter->drawLine(p0, p1);
                painter->drawLine(p1, p2);
                painter->drawLine(p2, p0);
            }
        }
    }

    if(m_RenderVertices)
    {
        // paint vertices
        for(const auto& vert : vertices)
        {
            QPointF vert_pos(vert->getPosition().x(), vert->getPosition().y());

            const Vec4f& vert_color = vert->getColor();
            paint_color = QColor::fromRgbF(vert_color[0], vert_color[1], vert_color[2], vert_color[3]);

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(paint_color));
            painter->drawEllipse(trans(vert_pos), point_size_px, point_size_px);
        }
    }
}

void Renderer::updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp)
{
    Mat3f V = vp->getCamera()->getViewMatrix();
    Mat3f P = vp->getProjectionMatrix();
    
    m_DeviceMat = renderTarget->getDeviceMatrix();
    m_InvDeviceMat = m_DeviceMat.inverse();

    m_TransMat = m_DeviceMat * P * V;
    m_InvTransMat = m_TransMat.inverse();
}

QPointF Renderer::trans(const QPointF& point)
{
    Vec3f res = m_TransMat * Vec3f(point.x(), point.y(), 1.0f);

    return QPointF(res.x(), res.y());
}

QPointF Renderer::transToDevice(const QPointF& point)
{
    Vec3f res = m_DeviceMat * Vec3f(point.x(), point.y(), 1.0f);

    return QPointF(res.x(), res.y());
}
