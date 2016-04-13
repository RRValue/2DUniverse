#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include "HalfEdge2D/Base/Vector.h"

#include <set>

class Mesh;
class Point;
class Circle;
class Line;
class CubicBezier;
class QuadraticBezier;

class Scene
{
public:
    Scene();
    ~Scene();

    void clear();

    const std::set<Mesh* const>& getMeshes() const;
    const std::set<Point* const>& getPoints() const;
    const std::set<Circle* const>& getCircles() const;
    const std::set<Line* const>& getLines() const;
    const std::set<QuadraticBezier* const>& getQuadraticBeziers() const;
    const std::set<CubicBezier* const>& getCubicBeziers() const;

    void setMeshes(const std::set<Mesh* const>& meshes);
    void setPoints(const std::set<Point* const>& points);
    void setCircles(const std::set<Circle* const>& circles);
    void setLines(const std::set<Line* const>& lines);
    void setQuadraticBeziers(const std::set<QuadraticBezier* const>& beziers);
    void setCubicBeziers(const std::set<CubicBezier* const>& beziers);

    void addMesh(Mesh* const mesh);
    void addPoint(Point* const point);
    void addCircle(Circle* const circle);
    void addLine(Line* const line);
    void addQuadraticBeziers(QuadraticBezier* const bezier);
    void addCubicBeziers(CubicBezier* const bezier);

    void removeMesh(Mesh* const mesh);
    void removePoint(Point* const point);
    void removeCircle(Circle* const circle);
    void removeLine(Line* const line);
    void removeQuadraticBeziers(QuadraticBezier* const bezier);
    void removeCubicBeziers(CubicBezier* const bezier);

private:
    float m_PointSize;

    std::set<Mesh* const> m_Meshes;
    std::set<Point* const> m_Points;
    std::set<Circle* const> m_Circles;
    std::set<Line* const> m_Lines;
    std::set<QuadraticBezier* const> m_QuadraticBeziers;
    std::set<CubicBezier* const> m_CubicBeziers;
};

#endif //_SCENE_SCENE_H_
