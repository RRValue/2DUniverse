#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include <QtGui/QPainter>

#include <QtWidgets/QWidget>

Renderer::Renderer()
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

void Renderer::addPaintTarget(QPaintTarget* const paintTarget)
{
    if(paintTarget == nullptr)
        return;

    if(m_PaintTargets.find(paintTarget) != m_PaintTargets.end())
        return;

    m_PaintTargets.insert(paintTarget);
}

void Renderer::removePaintTarget(QPaintTarget* const paintTarget)
{
    if(paintTarget == nullptr)
        return;

    if(m_PaintTargets.find(paintTarget) == m_PaintTargets.end())
        return;

    m_PaintTargets.erase(m_PaintTargets.find(paintTarget));
}

void Renderer::render()
{
    for(const auto& pt : m_PaintTargets)
        pt->update();
}

void Renderer::render(QPaintEvent* const event, QPaintTarget* const paintTarget)
{
    QPainter painter(paintTarget);

    painter.setRenderHint(QPainter::Antialiasing, true);

    for(const auto& vp : paintTarget->getViewPorts())
    {
        updateMatrices(paintTarget, vp);

        // add clipping
        const QRectF& vp_size = vp->getSize();
        const QSizeF& rt_size = paintTarget->getSize();

        QPointF bl = transToDevice(vp_size.bottomLeft());
        QPointF tr = transToDevice(vp_size.topRight());

        QRect clip_region((int)(bl.x() + 0.5f), (int)(bl.y() + 0.5f), (int)(tr.x() - bl.x() + 0.5f), (int)(tr.y() - bl.y() + 0.5f));

        painter.setClipRegion(clip_region);
        painter.setClipping(true);

        painter.fillRect(clip_region, Qt::GlobalColor::white);

        painter.setPen(Qt::GlobalColor::black);

        for(const auto& p : m_Scene->getPoints())
        {
            QPointF ref = trans(QPointF(0.0f, 0.0f));
            QPointF tar = trans(QPointF(m_Scene->getPointSize(), 0.0f));

            float point_size_px = (tar - ref).manhattanLength();

            painter.drawEllipse(trans(p), point_size_px, point_size_px);
        }

        painter.drawLine(trans(QPointF(-1.0f, 0.0f)), trans(QPointF(1.0f, 0.0f)));
        painter.drawLine(trans(QPointF(0.0f, -1.0f)), trans(QPointF(0.0f, 1.0f)));

        // render Viewport
        painter.setPen(QPen(Qt::GlobalColor::black));

        QPointF p0 = transToDevice(QPointF(vp_size.left(), vp_size.bottom()));
        QPointF p1 = transToDevice(QPointF(vp_size.right(), vp_size.bottom()));
        QPointF p2 = transToDevice(QPointF(vp_size.left(), vp_size.top()));
        QPointF p3 = transToDevice(QPointF(vp_size.right(), vp_size.top()));
        /*QPointF p2 = transToDevice(QPointF(0.0f, 0.0f));
        QPointF p3 = transToDevice(QPointF(0.0f, 0.0f));*/

        painter.drawLine(p0, p1);
        painter.drawLine(p0, p2);

        painter.drawLine(p1, p3);
        painter.drawLine(p2, p3);
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
