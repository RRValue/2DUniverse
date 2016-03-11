#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include <QtCore/QSize>
#include <QtCore/QVector>
#include <QtCore/QPointF>

class Camera;

class Scene
{
public:
    Scene();
    ~Scene();

    Camera* const getCamera() const;
    void setCamera(Camera* const camera);

    bool hasPoints() const;
    void setPointPos(const size_t& idx, const QPointF& pos);
    void addPoint(const QPointF& pos);
    const QPointF& getPoint(const size_t& idx) const;
    const QVector<QPointF>& getPoints() const;
    int getPointAtPos(const QPointF& pos) const;
    const float& getPointSize() const;

    bool inCanvas(const QPoint& point) const;
    QPoint keepInCanvas(const QPoint& point) const;

    QPointF toView(const QPointF& p) const;
    QPointF fromView(const QPointF& p) const;

    QPointF toDeviceCoords(const QPointF& point) const;
    QPointF fromDeviceCoords(const QPointF& point) const;

    QPointF toWidgetCoords(const QPointF& point) const;
    QPointF fromWidgetCoords(const QPointF& point) const;

    QPointF transform(const QPointF& point) const;
    QPointF invTransform(const QPointF& point) const;

private:
    Camera* m_Camera;

    float m_PointSizePx;
    QVector<QPointF> m_Points;
};

#endif //_SCENE_SCENE_H_
