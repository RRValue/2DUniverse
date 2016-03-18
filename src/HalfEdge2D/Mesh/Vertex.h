#ifndef _MESH_VERTEX_H_
#define _MESH_VERTEX_H_

#include "HalfEdge2D/Base/Vector.h"

class Vertex
{
public:
    Vertex();
    virtual ~Vertex();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const Vec2f& getPosition() const;
    const Vec2f& getNormal() const;
    const Vec4f& getColor() const;

    // setter
    void setPosition(const Vec2f& position);
    void setNormal(const Vec2f& normal);
    void setColor(const Vec4f& color);

private:
    Vec2f m_Position;
    Vec2f m_Normal;
    Vec4f m_Color;
};

#endif //_MESH_VERTEX_H_
