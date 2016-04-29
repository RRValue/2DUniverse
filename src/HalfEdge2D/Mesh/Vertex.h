#ifndef _MESH_VERTEX_H_
#define _MESH_VERTEX_H_

#include "HalfEdge2D/Base/Vector.h"

class Vertex
{
public:
    Vertex();
    Vertex(const Vertex& other);
    virtual ~Vertex();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const Vec2f& getPosition() const;
    const Vec2f& getNormal() const;
    const Vec4f& getColour() const;

    // setter
    void setPosition(const Vec2f& position);
    void setNormal(const Vec2f& normal);
    void setColour(const Vec4f& colour);

private:
    Vec2f m_Position;
    Vec2f m_Normal;
    Vec4f m_Colour;
};

#endif //_MESH_VERTEX_H_
