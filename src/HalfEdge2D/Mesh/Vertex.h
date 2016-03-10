#ifndef _MESH_VERTEX_H_
#define _MESH_VERTEX_H_

#include <Eigen\Dense>

typedef Eigen::Matrix<float, 2, 1>  Vec2f;

class Vertex
{
public:
    Vertex();
    ~Vertex();

private:
    Vec2f m_Position;
    Vec2f m_Normal;
};

#endif //_MESH_VERTEX_H_
