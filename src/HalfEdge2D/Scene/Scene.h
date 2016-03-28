#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include "HalfEdge2D/Base/Vector.h"

#include <vector>

typedef std::vector<Vec2f, Eigen::aligned_allocator<Vec2f>> PointVector;

class Mesh;

class Scene
{
public:
    Scene();
    ~Scene();

    Mesh* const getMesh() const;
    const PointVector& getPoints() const;

    void setMesh(Mesh* const mesh);
    void setPoints(const PointVector& points);

private:
    float m_PointSize;

    Mesh* m_Mesh;
    PointVector m_Points;
};

#endif //_SCENE_SCENE_H_
