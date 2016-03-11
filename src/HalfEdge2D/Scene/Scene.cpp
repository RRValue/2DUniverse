#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Scene/Camera.h"

Scene::Scene() : m_Camera(nullptr)
{

}

Scene::~Scene()
{

}

Camera* const Scene::getCamera() const
{
    return m_Camera;
}

void Scene::setCamera(Camera* const camera)
{
    m_Camera = camera;
}
