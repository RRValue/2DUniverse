#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include "HalfEdge2D/Base/Vector.h"

#include <set>

class Mesh;
class Point;

class Scene
{
public:
    Scene();
    ~Scene();

    void clear();

    Mesh* const getMesh() const;
    const std::set<Point* const>& getPoints() const;

    void setMesh(Mesh* const mesh);
    void setPoints(const std::set<Point* const>& points);

    void addPoint(Point* const point);
    void removePoint(Point* const point);

private:
    float m_PointSize;

    Mesh* m_Mesh;
    std::set<Point* const> m_Points;
};

#endif //_SCENE_SCENE_H_
