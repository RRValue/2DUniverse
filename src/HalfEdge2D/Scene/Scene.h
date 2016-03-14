#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

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

private:
    Camera* m_Camera;

    float m_PointSize;
    QVector<QPointF> m_Points;
};

#endif //_SCENE_SCENE_H_
