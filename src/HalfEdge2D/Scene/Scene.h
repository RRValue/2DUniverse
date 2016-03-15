#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include <QtCore/QVector>
#include <QtCore/QPointF>

#include <vector>

class Mesh;

class Scene
{
public:
    Scene();
    ~Scene();

    Mesh* const getMesh() const;

    void setMesh(Mesh* const mesh);

private:
    float m_PointSize;

    Mesh* m_Mesh;
};

#endif //_SCENE_SCENE_H_
