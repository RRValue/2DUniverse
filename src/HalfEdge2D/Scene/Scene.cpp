#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Mesh/Mesh.h"

#include "HalfEdge2D/Renderables/Point.h"

Scene::Scene()
{
    m_PointSize = 0.05f;
}

Scene::~Scene()
{

}

void Scene::clear()
{
    for(const auto& p : m_Points)
        delete p;

    m_Points.clear();
    m_Mesh->clear();
}

Mesh* const Scene::getMesh() const
{
    return m_Mesh;
}

const std::set<Point* const>& Scene::getPoints() const
{
    return m_Points;
}

void Scene::setMesh(Mesh* const mesh)
{
    if(mesh == nullptr)
        return;

    m_Mesh = mesh;
}

void Scene::setPoints(const std::set<Point* const>& points)
{
    for(const auto& p : points)
        addPoint(p);
}

void Scene::addPoint(Point* const point)
{
    if(point == nullptr)
        return;

    const auto& find_iter = m_Points.find(point);

    if(find_iter != m_Points.end())
        return;

    m_Points.insert(point);
}

void Scene::removePoint(Point* const point)
{
    if(point == nullptr)
        return;

    const auto& find_iter = m_Points.find(point);

    if(find_iter == m_Points.end())
        return;

    m_Points.erase(point);
}
