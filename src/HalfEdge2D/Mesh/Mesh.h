#ifndef _MESH_MESH_H_
#define _MESH_MESH_H_

#include "HalfEdge2D/Base/Vector.h"

#include <vector>

class Vertex;
class Triangle;

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    virtual void clear();

    // getter
    const std::vector<Vertex*>& getVertices() const;
    const std::vector<Triangle*>& getTriangles() const;

    Vertex* const getVertex(const size_t& idx) const;
    Triangle* const getTriangle(const size_t& idx) const;

    // vertices
    void addVertex();
    void addVertex(const Vec2f& pos);
    void addVertex(const Vec2f& pos, const Vec2f& normal);
    void removeVertex(const size_t& idx);

    // triangles
    void addTriangle(const size_t& idx0, const size_t& idx1, const size_t& idx2);
    void removeTriangle(const size_t& idx);

protected:
    virtual Vertex* allocateVertex();

protected:
    std::vector<Vertex*> m_Vertices;
    std::vector<Triangle*> m_Triangles;
};

#endif //_MESH_MESH_H_
