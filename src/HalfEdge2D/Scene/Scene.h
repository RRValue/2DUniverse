#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include "HalfEdge2D/Base/Vector.h"

#include <set>

class Mesh;
class Point;
class Circle;
class Line;

class Scene
{
public:
    Scene();
    ~Scene();

    void clear();

    Mesh* const getMesh() const;
    const std::set<Point* const>& getPoints() const;
    const std::set<Circle* const>& getCircles() const;
    const std::set<Line* const>& getLines() const;

    void setMesh(Mesh* const mesh);
    void setPoints(const std::set<Point* const>& points);
    void setCircles(const std::set<Circle* const>& circles);
    void setLines(const std::set<Line* const>& lines);

    void addPoint(Point* const point);
    void addCircle(Circle* const circle);
    void addLine(Line* const line);
    void removePoint(Point* const point);
    void removeCircle(Circle* const circle);
    void removeLine(Line* const line);

private:
    float m_PointSize;

    Mesh* m_Mesh;
    std::set<Point* const> m_Points;
    std::set<Circle* const> m_Circles;
    std::set<Line* const> m_Lines;
};

#endif //_SCENE_SCENE_H_
