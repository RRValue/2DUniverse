#ifndef _MESH_VERTEX_H_
#define _MESH_VERTEX_H_

#include "2DUniverse/Base/Vector.h"

class Vertex
{
public:
    Vertex();
    Vertex(const Vertex& other);
    virtual ~Vertex();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const Vec2d& getPosition() const;
    const Vec2d& getNormal() const;
    const Vec4f& getColour() const;

    // setter
    void setPosition(const Vec2d& position);
    void setNormal(const Vec2d& normal);
    void setColour(const Vec4f& colour);

private:
    Vec2d m_Position;
    Vec2d m_Normal;
    Vec4f m_Colour;
};

#endif //_MESH_VERTEX_H_
