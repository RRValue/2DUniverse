#include "HalfEdge2D/Scene/Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::clear()
{
    m_Meshes.clear();
    m_Points.clear();
    m_Circles.clear();
    m_Lines.clear();
    m_CubicBeziers.clear();
    m_Splines.clear();
}

const std::set<Mesh*>& Scene::getMeshes() const
{
    return m_Meshes;
}

const std::set<Point*>& Scene::getPoints() const
{
    return m_Points;
}

const std::set<Circle*>& Scene::getCircles() const
{
    return m_Circles;
}

const std::set<Line*>& Scene::getLines() const
{
    return m_Lines;
}

const std::set<QuadraticBezier*>& Scene::getQuadraticBeziers() const
{
    return m_QuadraticBeziers;
}

const std::set<CubicBezier*>& Scene::getCubicBeziers() const
{
    return m_CubicBeziers;
}

const std::set<Spline*>& Scene::getSplines() const
{
    return m_Splines;
}

void Scene::setMeshes(const std::set<Mesh*>& meshes)
{
    m_Meshes = meshes;
}

void Scene::setPoints(const std::set<Point*>& points)
{
    m_Points = points;
}

void Scene::setCircles(const std::set<Circle*>& circles)
{
    m_Circles = circles;
}

void Scene::setLines(const std::set<Line*>& lines)
{
    m_Lines = lines;
}

void Scene::setQuadraticBeziers(const std::set<QuadraticBezier*>& beziers)
{
    m_QuadraticBeziers = beziers;
}

void Scene::setCubicBeziers(const std::set<CubicBezier*>& beziers)
{
    m_CubicBeziers = beziers;
}

void Scene::setSplines(const std::set<Spline*>& splines)
{
    m_Splines = splines;
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

void Scene::addQuadraticBeziers(QuadraticBezier* const bezier)
{
    if(bezier == nullptr)
        return;

    const auto& find_iter = m_QuadraticBeziers.find(bezier);

    if(find_iter != m_QuadraticBeziers.end())
        return;

    m_QuadraticBeziers.insert(bezier);
}

void Scene::addCubicBeziers(CubicBezier* const bezier)
{
    if(bezier == nullptr)
        return;

    const auto& find_iter = m_CubicBeziers.find(bezier);

    if(find_iter != m_CubicBeziers.end())
        return;

    m_CubicBeziers.insert(bezier);
}

void Scene::addSpline(Spline* const spline)
{
    if(spline == nullptr)
        return;

    const auto& find_iter = m_Splines.find(spline);

    if(find_iter != m_Splines.end())
        return;

    m_Splines.insert(spline);
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

void Scene::removeQuadraticBeziers(QuadraticBezier* const bezier)
{
    if(bezier == nullptr)
        return;

    const auto& find_iter = m_QuadraticBeziers.find(bezier);

    if(find_iter == m_QuadraticBeziers.end())
        return;

    m_QuadraticBeziers.erase(find_iter);
}

void Scene::removeCubicBeziers(CubicBezier* const bezier)
{
    if(bezier == nullptr)
        return;

    const auto& find_iter = m_CubicBeziers.find(bezier);

    if(find_iter == m_CubicBeziers.end())
        return;

    m_CubicBeziers.erase(find_iter);
}

void Scene::removeSpline(Spline* const spline)
{
    if(spline == nullptr)
        return;

    const auto& find_iter = m_Splines.find(spline);

    if(find_iter == m_Splines.end())
        return;

    m_Splines.erase(find_iter);
}
