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
#include "HalfEdge2D/Renderables/QuadraticBezier.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"
#include "HalfEdge2D/Renderables/Spline.h"

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
    m_Scene = nullptr;
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
    renderMeshes(painter, scene->getMeshes());
    renderCircles(painter, scene->getCircles());
    renderLines(painter, scene->getLines());
    renderQuadraticBeziers(painter, scene->getQuadraticBeziers());
    renderCubicBeziers(painter, scene->getCubicBeziers());
    renderSplines(painter, scene->getSplines());
    renderPoints(painter, scene->getPoints());
}

void Renderer::renderMeshes(QPainter* const painter, const std::set<Mesh* const>& meshes)
{
    for(const auto& m : meshes)
        renderMesh(painter, m);
}

void Renderer::renderMesh(QPainter* const painter, Mesh* const mesh)
{
    if(mesh == nullptr)
        return;

    if(!mesh->isVisible())
        return;

    // get point point size to paint
    QPointF ref = trans(QPointF(0.0f, 0.0f));
    QPointF tar = trans(QPointF(m_PointSize, 0.0f));
    QPointF val = tar - ref;

    float point_size_px = std::sqrt((val.x() * val.x()) + (val.y() * val.y()));

    const std::vector<Vertex*>& vertices = mesh->getVertices();
    const std::vector<Face*>& faces = mesh->getFaces();

    QColor paint_color;

    if(m_RenderTriangles)
    {
        Vec2d vp, vps;
        QPointF p, ps;

        // paint traingles
        for(const auto& face : faces)
        {
            // fill triangle
            const Vec4f& tris_color = face->getColour();
            paint_color = QColor::fromRgbF(tris_color[0], tris_color[1], tris_color[2], tris_color[3]);

            QPainterPath path;

            const std::vector<size_t>& idxs = face->getVertIds();

            vps = vertices[face->getVertIdx(0)]->getPosition();
            ps = trans(QPointF(vps.x(), vps.y()));

            path.moveTo(ps);
            
            for(size_t i = 1; i < idxs.size(); i++)
            {
                vp = vertices[face->getVertIdx(i)]->getPosition();
                p = trans(QPointF(vp.x(), vp.y()));

                path.lineTo(p);
            }

            path.lineTo(ps);

            painter->setPen(Qt::NoPen);
            painter->fillPath(path, QBrush(paint_color));
        }
    }

    if(m_RenderTrianglesEdges)
    {
        Vec2d vp0, vp1;
        QPointF p0, p1;

        // paint traingles edges
        for(const auto& face : faces)
        {
            painter->setPen(Qt::SolidLine);
            painter->setPen(Qt::black);

            const std::vector<size_t>& idxs = face->getVertIds();
            size_t num_idxs = idxs.size();

            for(size_t i = 0; i < num_idxs; i++)
            {
                Vec2d vp0 = vertices[face->getVertIdx(i)]->getPosition();
                Vec2d vp1 = vertices[face->getVertIdx((i + 1) % num_idxs)]->getPosition();

                QPointF p0 = trans(QPointF(vp0.x(), vp0.y()));
                QPointF p1 = trans(QPointF(vp1.x(), vp1.y()));

                painter->drawLine(p0, p1);
            }
        }
    }

    if(m_RenderVertices)
    {
        // paint vertices
        for(const auto& vert : vertices)
        {
            QPointF vert_pos(vert->getPosition().x(), vert->getPosition().y());

            const Vec4f& vert_color = vert->getColour();
            paint_color = QColor::fromRgbF(vert_color[0], vert_color[1], vert_color[2], vert_color[3]);

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(paint_color));
            painter->drawEllipse(trans(vert_pos), point_size_px, point_size_px);
        }
    }
}

void Renderer::renderPoints(QPainter* const painter, const std::set<Point* const>& points)
{
    for(const auto& p : points)
        renderPoint(painter, p);
}

void Renderer::renderPoint(QPainter* const painter, Point* point)
{
    if(point == nullptr)
        return;

    if(!point->isVisible())
        return;

    QPointF ref = trans(QPointF(0.0f, 0.0f));
    QPointF tar = trans(QPointF(point->getSize(), 0.0f));
    QPointF val = tar - ref;

    float point_size_px = std::sqrt((val.x() * val.x()) + (val.y() * val.y()));

    const Vec2d& pos = point->getPosition();
    const Vec4f& col = point->getColour();

    QPointF vert_pos(pos[0], pos[1]);

    QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(paint_color));
    painter->drawEllipse(trans(vert_pos), point_size_px, point_size_px);
}

void Renderer::renderCircles(QPainter* const painter, const std::set<Circle* const>& circles)
{
    for(const auto& c : circles)
        renderCircle(painter, c);
}

void Renderer::renderCircle(QPainter* const painter, Circle* const circle)
{
    if(circle == nullptr)
        return;

    if(!circle->isVisible())
        return;

    float radius_px = getPixelSize(circle->getRadius());
    float thickness_px = getPixelSize(circle->getThickness());

    const Vec2d& pos = circle->getPosition();
    const Vec4f& col = circle->getColour();

    QPointF circle_pos(pos[0], pos[1]);

    QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

    painter->setPen(QPen(paint_color, thickness_px));
    painter->setBrush(Qt::BrushStyle::NoBrush);
    painter->drawEllipse(trans(circle_pos), radius_px, radius_px);
}

void Renderer::renderLines(QPainter* const painter, const std::set<Line* const>& lines)
{
    for(const auto& l : lines)
        renderLine(painter, l);
}

void Renderer::renderLine(QPainter* const painter, Line* const line)
{
    if(line == nullptr)
        return;

    if(!line->isVisible())
        return;

    // get thickness
    float thickness_px = getPixelSize(line->getThickness());

    // get colour
    const Vec4f& col = line->getColour();

    QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

    painter->setPen(QPen(paint_color, thickness_px));
    painter->setBrush(Qt::BrushStyle::NoBrush);

    renderLine(painter, *(Line2D*)line);
}

void Renderer::renderLine(QPainter* const painter, const Line2D& line)
{
    // get points
    const Line::BezierPointsType& points = line.getPoints();

    // get bb
    std::array<QPointF, 2> pts;

    for(size_t i = 0; i < 2; i++)
        pts[i] = trans(QPointF(points(0, i), points(1, i)));

    painter->drawLine(pts[0], pts[1]);
}

void Renderer::renderQuadraticBeziers(QPainter* const painter, const std::set<QuadraticBezier* const>& beziers)
{
    for(const auto& b : beziers)
        renderQuadraticBezier(painter, b);
}

void Renderer::renderQuadraticBezier(QPainter* const painter, QuadraticBezier* const bezier)
{
    if(bezier == nullptr)
        return;

    if(!bezier->isVisible())
        return;

    // get thickness
    float thickness_px = getPixelSize(bezier->getThickness());
    // get colour
    const Vec4f& col = bezier->getColour();
    
    QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

    // set paint color
    painter->setPen(QPen(paint_color, thickness_px));
    painter->setBrush(Qt::BrushStyle::NoBrush);

    renderQuadraticBezier(painter, *(QBezier2D*)bezier);
}

void Renderer::renderQuadraticBezier(QPainter* const painter, const QBezier2D& bezier)
{
    // get points
    const QuadraticBezier::BezierPointsType& points = bezier.getPoints();

    // get bb
    std::array<QPointF, 3> bb_points;

    for(size_t i = 0; i < 3; i++)
        bb_points[i] = trans(QPointF(points(0, i), points(1, i)));

    float pos_inf = std::numeric_limits<float>::infinity();
    float neg_inf = -pos_inf;

    QPointF min(pos_inf, pos_inf);
    QPointF max(neg_inf, neg_inf);

    for(const auto& bb_p : bb_points)
    {
        if(bb_p.x() < min.x()) min.setX(bb_p.x());
        if(bb_p.y() < min.y()) min.setY(bb_p.y());

        if(bb_p.x() > max.x()) max.setX(bb_p.x());
        if(bb_p.y() > max.y()) max.setY(bb_p.y());
    }

    // get alpha steps
    QPointF diag = max - min;
    float bb_diag_length = std::sqrt((diag.x() * diag.x()) + (diag.y() * diag.y()));
    float max_bezier_length = 1.5f * bb_diag_length;

    // defines num lines when bezier length is 1000 px
    int max_ration = 100;
    float coef_a = (-1.0f + (float)max_ration) / 999.0f;
    float coef_b = (1000.0f - (float)max_ration) / 999.0f;

    unsigned int num_lines = (unsigned int)(((coef_a * max_bezier_length) + coef_b) + 0.5f);
    float alpha_step = 1.0f / (float)num_lines;

    // build lines
    QVector<QLineF> lines;

    double alpha = 0.0;

    for(unsigned int i = 0; i < num_lines; i++)
    {
        Vec2d bp0 = bezier.pointAt(alpha);
        Vec2d bp1 = bezier.pointAt(alpha + alpha_step);

        lines.push_back(QLineF(trans(QPointF(bp0[0], bp0[1])), trans(QPointF(bp1[0], bp1[1]))));

        alpha += alpha_step;
    }

    // paint bezier
    painter->drawLines(lines);
}

void Renderer::renderCubicBeziers(QPainter* const painter, const std::set<CubicBezier* const>& beziers)
{
    for(const auto& b : beziers)
        renderCubicBezier(painter, b);
}

void Renderer::renderCubicBezier(QPainter* const painter, CubicBezier* const bezier)
{
    if(bezier == nullptr)
        return;

    if(!bezier->isVisible())
        return;

    // get thickness
    float thickness_px = getPixelSize(bezier->getThickness());

    // get colour
    const Vec4f& col = bezier->getColour();

    QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

    painter->setPen(QPen(paint_color, thickness_px));
    painter->setBrush(Qt::BrushStyle::NoBrush);

    renderCubicBezier(painter, *(CBezier2D*)bezier);
}

void Renderer::renderCubicBezier(QPainter* const painter, const CBezier2D& bezier)
{
    // get points
    const CubicBezier::BezierPointsType& points = bezier.getPoints();

    // get bb
    std::array<QPointF, 4> bb_points;

    for(size_t i = 0; i < 4; i++)
        bb_points[i] = trans(QPointF(points(0, i), points(1, i)));

    float pos_inf = std::numeric_limits<float>::infinity();
    float neg_inf = -pos_inf;

    QPointF min(pos_inf, pos_inf);
    QPointF max(neg_inf, neg_inf);

    for(const auto& bb_p : bb_points)
    {
        if(bb_p.x() < min.x()) min.setX(bb_p.x());
        if(bb_p.y() < min.y()) min.setY(bb_p.y());

        if(bb_p.x() > max.x()) max.setX(bb_p.x());
        if(bb_p.y() > max.y()) max.setY(bb_p.y());
    }

    // get alpha steps
    QPointF diag = max - min;
    float bb_diag_length = std::sqrt((diag.x() * diag.x()) + (diag.y() * diag.y()));
    float max_bezier_length = 2.5f * bb_diag_length;

    // defines num lines when bezier length is 1000 px
    int max_ration = 100;
    float coef_a = (-1.0f + (float)max_ration) / 999.0f;
    float coef_b = (1000.0f - (float)max_ration) / 999.0f;

    unsigned int num_lines = (unsigned int)(((coef_a * max_bezier_length) + coef_b) + 0.5f);
    float alpha_step = 1.0f / (float)num_lines;

    // build lines
    QVector<QLineF> lines;

    double alpha = 0.0;

    for(unsigned int i = 0; i < num_lines; i++)
    {
        Vec2d bp0 = bezier.pointAt(alpha);
        Vec2d bp1 = bezier.pointAt(alpha + alpha_step);

        lines.push_back(QLineF(trans(QPointF(bp0[0], bp0[1])), trans(QPointF(bp1[0], bp1[1]))));

        alpha += alpha_step;
    }

    // paint bezier
    painter->drawLines(lines);
}

void Renderer::renderSplines(QPainter* const painter, const std::set<Spline* const>& splines)
{
    for(const auto& s : splines)
        renderSpline(painter, s);
}

void Renderer::renderSpline(QPainter* const painter, Spline* const spline)
{
    if(spline == nullptr)
        return;

    if(!spline->isVisible())
        return;

    // get thickness
    float thickness_px = getPixelSize(spline->getThickness());

    // get colour
    const Vec4f& col = spline->getColour();

    QColor paint_color = QColor::fromRgbF(col[0], col[1], col[2], col[3]);

    painter->setPen(QPen(paint_color, thickness_px));
    painter->setBrush(Qt::BrushStyle::NoBrush);

    for(const auto& s : spline->getSegements())
        if(s.m_Active)
            renderCubicBezier(painter, s.m_Bezier);
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

float Renderer::getPixelSize(const float& value)
{
    QPointF ref = trans(QPointF(0.0f, 0.0f));
    QPointF tar = trans(QPointF(value, 0.0f));
    QPointF val = tar - ref;

    return std::sqrt((val.x() * val.x()) + (val.y() * val.y()));
}
