#include "HalfEdge2D/Scene/Scene.h"

Scene::Scene()
{
    m_PointSize = 0.05f;
}

Scene::~Scene()
{

}

Mesh* const Scene::getMesh() const
{
    return m_Mesh;
}

const PointVector& Scene::getPoints() const
{
    return m_Points;
}

void Scene::setMesh(Mesh* const mesh)
{
    if(mesh == nullptr)
        return;

    m_Mesh = mesh;
}

void Scene::setPoints(const PointVector& points)
{
    m_Points = points;
}
