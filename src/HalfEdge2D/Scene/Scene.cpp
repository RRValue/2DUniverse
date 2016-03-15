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

void Scene::setMesh(Mesh* const mesh)
{
    if(mesh == nullptr)
        return;

    m_Mesh = mesh;
}
