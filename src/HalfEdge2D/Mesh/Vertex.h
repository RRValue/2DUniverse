#ifndef _MESH_VERTEX_H_
#define _MESH_VERTEX_H_

#include "HalfEdge2D/Base/Vector.h"

class Vertex
{
public:
    Vertex();
    ~Vertex();

    // getter
    const Vec2f& getPosition() const;
    const Vec2f& getNormal() const;

    // setter
    void setPosition(const Vec2f& position);
    void setNormal(const Vec2f& normal);

private:
    Vec2f m_Position;
    Vec2f m_Normal;
};

#endif //_MESH_VERTEX_H_
