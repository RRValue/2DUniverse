#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Mesh/Mesh.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Circle.h"
#include "HalfEdge2D/Renderables/Line.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"

Scene::Scene()
{
    m_PointSize = 0.05f;
}

Scene::~Scene()
{

}

void Scene::clear()
{
    // clear meshes
    for(const auto& m : m_Meshes)
        delete m;

    m_Meshes.clear();

    // clear points
    for(const auto& p : m_Points)
        delete p;

    m_Points.clear();

    // clear circles
    for(const auto& c : m_Circles)
        delete c;

    m_Circles.clear();

    // clear lines
    for(const auto& l : m_Lines)
        delete l;

    m_Lines.clear();

    // clear cubic beziers
    for(const auto& b : m_CubicBeziers)
        delete b;

    m_CubicBeziers.clear();
}

const std::set<Mesh* const>& Scene::getMeshes() const
{
    return m_Meshes;
}

const std::set<Point* const>& Scene::getPoints() const
{
    return m_Points;
}

const std::set<Circle* const>& Scene::getCircles() const
{
    return m_Circles;
}

const std::set<Line* const>& Scene::getLines() const
{
    return m_Lines;
}

const std::set<CubicBezier* const>& Scene::getCubicBeziers() const
{
    return m_CubicBeziers;
}

void Scene::setMeshes(const std::set<Mesh* const>& meshes)
{
    m_Meshes = meshes;
}

void Scene::setPoints(const std::set<Point* const>& points)
{
    for(const auto& p : points)
        addPoint(p);
}

void Scene::setCircles(const std::set<Circle* const>& circles)
{
    for(const auto& c : circles)
        addCircle(c);
}

void Scene::setLines(const std::set<Line* const>& lines)
{
    for(const auto& l : lines)
        addLine(l);
}

void Scene::setCubicBeziers(const std::set<CubicBezier* const>& cubicBeziers)
{
    m_CubicBeziers = cubicBeziers;
}

void Scene::addMesh(Mesh* const mesh)
{
    if(mesh == nullptr)
        return;

    const auto& find_iter = m_Meshes.find(mesh);

    if(find_iter != m_Meshes.end())
        return;

    m_Meshes.insert(mesh);
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

void Scene::addCircle(Circle* const circle)
{
    if(circle == nullptr)
        return;

    const auto& find_iter = m_Circles.find(circle);

    if(find_iter != m_Circles.end())
        return;

    m_Circles.insert(circle);
}

void Scene::addLine(Line* const line)
{
    if(line == nullptr)
        return;

    const auto& find_iter = m_Lines.find(line);

    if(find_iter != m_Lines.end())
        return;

    m_Lines.insert(line);
}

void Scene::addCubicBeziers(CubicBezier* const cubicBezier)
{
    if(cubicBezier == nullptr)
        return;

    const auto& find_iter = m_CubicBeziers.find(cubicBezier);

    if(find_iter != m_CubicBeziers.end())
        return;

    m_CubicBeziers.insert(cubicBezier);
}

void Scene::removeMesh(Mesh* const mesh)
{
    if(mesh == nullptr)
        return;

    const auto& find_iter = m_Meshes.find(mesh);

    if(find_iter == m_Meshes.end())
        return;

    m_Meshes.erase(find_iter);
}

void Scene::removePoint(Point* const point)
{
    if(point == nullptr)
        return;

    const auto& find_iter = m_Points.find(point);

    if(find_iter == m_Points.end())
        return;

    m_Points.erase(find_iter);
}

void Scene::removeCircle(Circle* const circle)
{
    if(circle == nullptr)
        return;

    const auto& find_iter = m_Circles.find(circle);

    if(find_iter == m_Circles.end())
        return;

    m_Circles.erase(find_iter);
}

void Scene::removeLine(Line* const line)
{
    if(line == nullptr)
        return;

    const auto& find_iter = m_Lines.find(line);

    if(find_iter == m_Lines.end())
        return;

    m_Lines.erase(find_iter);
}

void Scene::removeCubicBeziers(CubicBezier* const cubicBezier)
{
    if(cubicBezier == nullptr)
        return;

    const auto& find_iter = m_CubicBeziers.find(cubicBezier);

    if(find_iter == m_CubicBeziers.end())
        return;

    m_CubicBeziers.erase(find_iter);
}
