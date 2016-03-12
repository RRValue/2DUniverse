#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/Canvas.h"

Scene::Scene() : m_Camera(nullptr)
{
    m_PointSizePx = 5.0f;
}

Scene::~Scene()
{

}

bool Scene::hasPoints() const
{
    return !m_Points.empty();
}

void Scene::setPointPos(const size_t& idx, const QPointF& pos)
{
    m_Points[idx] = pos;
}

void Scene::addPoint(const QPointF& pos)
{
    m_Points.push_back(pos);
}

const QPointF& Scene::getPoint(const size_t& idx) const
{
    return m_Points[idx];
}

const QVector<QPointF>& Scene::getPoints() const
{
    return m_Points;
}

const float& Scene::getPointSize() const
{
    return m_PointSizePx;
}

int Scene::getPointAtPos(const QPointF& pos) const
{
    if(!hasPoints())
        return -1;

    for(int i = 0; i < m_Points.size(); i++)
    {
        QPointF point_pos = transform(m_Points[i]);
        QPointF delta = point_pos - pos;
        float distance = std::sqrt(std::pow(delta.x(), 2) + std::pow(delta.y(), 2));

        if(distance < m_PointSizePx)
            return i;
    }

    return -1;
}

Camera* const Scene::getCamera() const
{
    return m_Camera;
}

void Scene::setCamera(Camera* const camera)
{
    m_Camera = camera;
}

QPointF Scene::toView(const QPointF& p) const
{
    const QPointF& cam_pos = m_Camera->getPosition();
    const QRectF& vp= m_Camera->getViewport();
    const float& zoom = m_Camera->getZoom();

    return QPointF(
        (p.x() - cam_pos.x()) * zoom * vp.width(),
        (p.y() - cam_pos.y()) * zoom * vp.height());
}

QPointF Scene::fromView(const QPointF& p) const
{
    const QPointF& cam_pos = m_Camera->getPosition();
    const QRectF& vp = m_Camera->getViewport();
    const float& zoom = m_Camera->getZoom();

    return QPointF(
        (p.x() / zoom * vp.width()) + cam_pos.x(),
        (p.y() / zoom * vp.height()) + cam_pos.y());
}

QPointF Scene::toDeviceCoords(const QPointF& point) const
{
    const QRectF& vp = m_Camera->getViewport();
    const QSize& size = m_Camera->getCanvas()->getSize();
    QSizeF sizef = QSizeF(float(size.width()), float(size.height()));

    float ar = (sizef.height() * vp.height()) / (sizef.width() * vp.width());
    
    return QPointF(
        ((point.x() * ar) + 1.0f) / 2.0f,
        (point.y() + 1.0f) / 2.0f);
}

QPointF Scene::fromDeviceCoords(const QPointF& point) const
{
    const QRectF& vp = m_Camera->getViewport();
    const QSize& size = m_Camera->getCanvas()->getSize();
    QSizeF sizef = QSizeF(float(size.width()), float(size.height()));

    float ar = (sizef.height() * vp.height()) / (sizef.width() * vp.width());
    
    return QPointF(
        ((point.x() * 2.0f) - 1.0f) / ar,
        (point.y() * 2.0f) - 1.0f);
}

QPointF Scene::toWidgetCoords(const QPointF& p) const
{
    const QRectF& vp = m_Camera->getViewport();
    const QSize& size = m_Camera->getCanvas()->getSize();
    QSizeF sizef = QSizeF(float(size.width()), float(size.height()));

    float Ax = sizef.width() * vp.width();
    float Ay = sizef.height() * vp.height();

    float Bx = vp.x() * sizef.width();
    float By = vp.y() * sizef.height();

    return QPointF(
        ((0.0f + p.x()) * Ax) + Bx,
        ((1.0f - p.y()) * Ay) + By);
}

QPointF Scene::fromWidgetCoords(const QPointF& p) const
{
    const QRectF& vp = m_Camera->getViewport();
    const QSize& size = m_Camera->getCanvas()->getSize();
    QSizeF sizef = QSizeF(float(size.width()), float(size.height()));

    float Ax = sizef.width() * vp.width();
    float Ay = sizef.height() * vp.height();

    float Bx = vp.x() * sizef.width();
    float By = vp.y() * sizef.height();

    return QPointF(
        0.0f + ((p.x() - Bx) / Ax),
        1.0f - ((p.y() - By) / Ay));
}

QPointF Scene::transform(const QPointF& point) const
{
    return toWidgetCoords(toDeviceCoords(toView(point)));
}

QPointF Scene::invTransform(const QPointF& point) const
{
    return fromView(fromDeviceCoords(fromWidgetCoords(point)));
}

bool Scene::inCanvas(const QPoint& point) const
{
    const QSize& canvas_size = m_Camera->getCanvas()->getSize();

    if(point.x() < 0 || point.x() >= canvas_size.width())
        return false;

    if(point.y() < 0 || point.y() >= canvas_size.height())
        return false;

    return true;
}

QPoint Scene::keepInCanvas(const QPoint& point) const
{
    const QSize& canvas_size = m_Camera->getCanvas()->getSize();
    QPoint pos = point;

    if(pos.x() > canvas_size.width())
        pos.setX(canvas_size.width());
    if(pos.x() < 0)
        pos.setX(0);

    if(pos.y() > canvas_size.height())
        pos.setY(canvas_size.height());
    if(pos.y() < 0)
        pos.setY(0);

    return pos;
}
