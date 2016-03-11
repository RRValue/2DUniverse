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
}
