#ifndef _MESH_MESH_H_
#define _MESH_MESH_H_

#include "HalfEdge2D/Base/Vector.h"

#include <vector>

class Vertex;

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    // getter
    const std::vector<Vertex*>& getVertices() const;
    Vertex* const getVertex(const size_t& idx) const;

    // vertices
    virtual void addVertex(const Vec2f& pos, const Vec2f& normal);
    void removeVertex(const size_t& idx);

protected:
    void addVertex(Vertex* const vertex);

protected:
    std::vector<Vertex*> m_Vertices;
};

#endif //_MESH_MESH_H_
