#include "HalfEdge2D/Scene/Scene.h"

Scene::Scene()
{
    m_PointSize = 0.05f;
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
    return m_PointSize;
}

int Scene::getPointAtPos(const QPointF& pos) const
{
    if(!hasPoints())
        return -1;

    for(int i = 0; i < m_Points.size(); i++)
    {
        QPointF point_pos = m_Points[i];
        QPointF delta = point_pos - pos;
        float distance = std::sqrt(std::pow(delta.x(), 2) + std::pow(delta.y(), 2));

        if(distance < m_PointSize)
            return i;
    }

    return -1;
}
