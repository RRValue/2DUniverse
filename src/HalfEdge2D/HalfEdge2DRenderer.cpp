#include "HalfEdge2D/HalfEdge2DRenderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/RenderTarget.h"

#include <QtGui/QPainter>

#include <QtWidgets/QWidget>

HalfEdge2DRenderer::HalfEdge2DRenderer()
{

}

HalfEdge2DRenderer::~HalfEdge2DRenderer()
{

}

void HalfEdge2DRenderer::setWidget(QWidget* const widget)
{
    if(widget == nullptr)
        return;

    m_Widget = widget;
}

void HalfEdge2DRenderer::setScene(Scene* const scene)
{
    if(scene == nullptr)
        return;

    m_Scene = scene;
}

void HalfEdge2DRenderer::render(QPaintEvent* const event, RenderTarget* const renderTarget)
{
    QPainter painter(m_Widget);

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

        //painter.fillRect(clip, Qt::GlobalColor::white);

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
        painter.setPen(QPen(Qt::GlobalColor::red));

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

void HalfEdge2DRenderer::updateMatrices(RenderTarget* const renderTarget, ViewPort* const vp)
{
    Mat3f V = vp->getCamera()->getViewMatrix();
    Mat3f P = vp->getProjectionMatrix();
    
    m_DeviceMat = renderTarget->getDeviceMatrix();
    m_InvDeviceMat = m_DeviceMat.inverse();

    m_TransMat = m_DeviceMat * P * V;
    m_InvTransMat = m_TransMat.inverse();
}

QPointF HalfEdge2DRenderer::trans(const QPointF& point)
{
    Vec3f res = m_TransMat * Vec3f(point.x(), point.y(), 1.0f);

    return QPointF(res.x(), res.y());
}

QPointF HalfEdge2DRenderer::transToDevice(const QPointF& point)
{
    Vec3f res = m_DeviceMat * Vec3f(point.x(), point.y(), 1.0f);

    return QPointF(res.x(), res.y());
}
