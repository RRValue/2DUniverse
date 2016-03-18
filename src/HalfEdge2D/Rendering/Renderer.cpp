#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/QWidgetTarget.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include "HalfEdge2D/Mesh/Mesh.h"
#include "HalfEdge2D/Mesh/Vertex.h"
#include "HalfEdge2D/Mesh/Triangle.h"

#include <QtGui/QPainter>

#include <QtWidgets/QWidget>

Renderer::Renderer() : m_PointSize(0.005f)
{

}

Renderer::~Renderer()
{

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
        pt->update();
}

void Renderer::render(QWidgetTarget* const widgetTarget)
{
    paint(widgetTarget, widgetTarget);
}

void Renderer::render(QPaintTarget* const paintTarget)
{
    paint(paintTarget, paintTarget);
}

void Renderer::render(QPaintEvent* const event, QWidgetTarget* const widgetTarget)
{
    paint(widgetTarget, widgetTarget);
}

void Renderer::paint(QPaintDevice* const paintDevice, RenderTarget* const renderTarget)
{
    QPainter painter(paintDevice);

    painter.setRenderHint(QPainter::Antialiasing, true);

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

        painter.fillRect(clip_region, Qt::GlobalColor::white);

        // render scene
        if(m_Scene != nullptr)
            renderScene(&painter, m_Scene);

        // render coordiante system
        painter.setPen(QPen(Qt::GlobalColor::red));
        painter.drawLine(trans(QPointF(-1.0f, 0.0f)), trans(QPointF(1.0f, 0.0f)));
        painter.drawLine(trans(QPointF(0.0f, -1.0f)), trans(QPointF(0.0f, 1.0f)));

        // render Viewport
        painter.setPen(QPen(Qt::GlobalColor::black));

        QPointF p0 = transToDevice(QPointF(vp_size.left(), vp_size.bottom()));
        QPointF p1 = transToDevice(QPointF(vp_size.right(), vp_size.bottom()));
        QPointF p2 = transToDevice(QPointF(vp_size.left(), vp_size.top()));
        QPointF p3 = transToDevice(QPointF(vp_size.right(), vp_size.top()));

        painter.drawLine(p0, p1);
        painter.drawLine(p0, p2);

        painter.drawLine(p1, p3);
        painter.drawLine(p2, p3);
    }
}

void Renderer::renderScene(QPainter* const painter, Scene* const scene)
{
    if(scene->getMesh() != nullptr)
        renderMesh(painter, m_Scene->getMesh());
}

void Renderer::renderMesh(QPainter* const painter, Mesh* const mesh)
{
    // get point point size to paint
    QPointF ref = trans(QPointF(0.0f, 0.0f));
    QPointF tar = trans(QPointF(m_PointSize, 0.0f));
    float point_size_px = (tar - ref).manhattanLength();

    const std::vector<Vertex*>& vertices = mesh->getVertices();
    const std::vector<Triangle*>& triangles = mesh->getTriangles();

    // paint traingles
    painter->setBrush(QBrush(Qt::GlobalColor::gray));

    QPolygonF triangle_poly(3);

    for(const auto& triangle : triangles)
    {
        Vec2f vp0 = vertices[triangle->data()[0]]->getPosition();
        Vec2f vp1 = vertices[triangle->data()[1]]->getPosition();
        Vec2f vp2 = vertices[triangle->data()[2]]->getPosition();

        triangle_poly[0] = trans(QPointF(vp0.x(), vp0.y()));
        triangle_poly[1] = trans(QPointF(vp1.x(), vp1.y()));
        triangle_poly[2] = trans(QPointF(vp2.x(), vp2.y()));

        painter->drawPolygon(triangle_poly);
    }

    // paint vertices
    painter->setBrush(QBrush(Qt::GlobalColor::black));

    for(const auto& p : vertices)
    {
        QPointF vert_pos(p->getPosition().x(), p->getPosition().y());
        
        painter->drawEllipse(trans(vert_pos), point_size_px, point_size_px);
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
