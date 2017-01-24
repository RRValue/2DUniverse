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
class Spline;

class Scene
{
public:
    Scene();
    ~Scene();

    void clear();

    const std::set<Mesh*>& getMeshes() const;
    const std::set<Point*>& getPoints() const;
    const std::set<Circle*>& getCircles() const;
    const std::set<Line*>& getLines() const;
    const std::set<QuadraticBezier*>& getQuadraticBeziers() const;
    const std::set<CubicBezier*>& getCubicBeziers() const;
    const std::set<Spline*>& getSplines() const;

    void setMeshes(const std::set<Mesh*>& meshes);
    void setPoints(const std::set<Point*>& points);
    void setCircles(const std::set<Circle*>& circles);
    void setLines(const std::set<Line*>& lines);
    void setQuadraticBeziers(const std::set<QuadraticBezier*>& beziers);
    void setCubicBeziers(const std::set<CubicBezier*>& beziers);
    void setSplines(const std::set<Spline*>& splines);

    void addMesh(Mesh* const mesh);
    void addPoint(Point* const point);
    void addCircle(Circle* const circle);
    void addLine(Line* const line);
    void addQuadraticBeziers(QuadraticBezier* const bezier);
    void addCubicBeziers(CubicBezier* const bezier);
    void addSpline(Spline* const spline);

    void removeMesh(Mesh* const mesh);
    void removePoint(Point* const point);
    void removeCircle(Circle* const circle);
    void removeLine(Line* const line);
    void removeQuadraticBeziers(QuadraticBezier* const bezier);
    void removeCubicBeziers(CubicBezier* const bezier);
    void removeSpline(Spline* const spline);

private:
    std::set<Mesh*> m_Meshes;
    std::set<Point*> m_Points;
    std::set<Circle*> m_Circles;
    std::set<Line*> m_Lines;
    std::set<QuadraticBezier*> m_QuadraticBeziers;
    std::set<CubicBezier*> m_CubicBeziers;
    std::set<Spline*> m_Splines;
};

#endif //_SCENE_SCENE_H_
