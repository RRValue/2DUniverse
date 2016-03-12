#include "HalfEdge2D/HalfEdge2DRenderer.h"

#include "HalfEdge2D/Scene/Scene.h"

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

void HalfEdge2DRenderer::render(QPaintEvent* const event)
{
    QPainter painter(m_Widget);

    for(const auto& p : m_Scene->getPoints())
        painter.drawEllipse(m_Scene->transform(p), m_Scene->getPointSize(), m_Scene->getPointSize());

    painter.drawLine(m_Scene->transform(QPointF(-1.0f, 0.0f)), m_Scene->transform(QPointF(1.0f, 0.0f)));
    painter.drawLine(m_Scene->transform(QPointF(0.0f, -1.0f)), m_Scene->transform(QPointF(0.0f, 1.0f)));

    int w = m_Widget->width() - 1;
    int h = m_Widget->height() - 1;

    int w_h = m_Widget->width() / 2;
    int h_h = m_Widget->height() / 2;

    painter.drawLine(QPoint(0, 0), QPoint(w, 0));
    painter.drawLine(QPoint(0, 0), QPoint(0, h));
    
    painter.drawLine(QPoint(w, 0), QPoint(w, h));
    painter.drawLine(QPoint(0, h), QPoint(w, h));

    painter.drawLine(QPoint(w_h, 0), QPoint(w_h, h));
    painter.drawLine(QPoint(0, h_h), QPoint(w, h_h));

    painter.setPen(QPen(Qt::GlobalColor::red));

    QPointF p0 = m_Scene->transform(m_Scene->invTransform(QPointF(0.0f, 0.0f)));
    QPointF p1 = m_Scene->transform(m_Scene->invTransform(QPointF((float)w, 0.0f)));
    QPointF p2 = m_Scene->transform(m_Scene->invTransform(QPointF(0.0f, (float)h)));
    QPointF p3 = m_Scene->transform(m_Scene->invTransform(QPointF((float)w, (float)h)));

    painter.drawLine(p0, p1);
    painter.drawLine(p0, p2);

    painter.drawLine(p1, p3);
    painter.drawLine(p2, p3);
}
