#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Mesh/Mesh.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Circle.h"
#include "HalfEdge2D/Renderables/Line.h"

Scene::Scene()
{
    m_PointSize = 0.05f;
}

Scene::~Scene()
{

}

void Scene::clear()
{
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

const std::set<Circle* const>& Scene::getCircles() const
{
    return m_Circles;
}

const std::set<Line* const>& Scene::getLines() const
{
    return m_Lines;
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
