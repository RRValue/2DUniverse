#ifndef _MESH_MESH_H_
#define _MESH_MESH_H_

#include "2DUniverse/Base/Vector.h"

#include "2DUniverse/Renderables/Renderable.h"

#include <vector>

class Vertex;
class Face;

class Mesh : public Renderable
{
public:
    Mesh();
    Mesh(const Mesh& other);
    virtual ~Mesh();

    Mesh& operator=(const Mesh& other);

    virtual void clear();

    // getter
    size_t getNumVertices() const;
    size_t getNumFaces() const;
    const std::vector<Vertex*>& getVertices() const;
    const std::vector<Face*>& getFaces() const;

    Vertex* const getVertex(const size_t& idx) const;
    Face* const getFace(const size_t& idx) const;

    // vertices
    void addVertex();
    void addVertex(const Vertex& vertex);
    void addVertex(const Vec2d& pos);
    void addVertex(const Vec2d& pos, const Vec2d& normal);
    void addVertex(const Vec2d& pos, const Vec2d& normal, const Vec4f& color);
    void removeVertex(const size_t& idx);

    // triangles
    void addFace(const Face& f);
    void addFace(const size_t& idx0, const size_t& idx1, const size_t& idx2);
    void addFace(const std::initializer_list<size_t>& idxs);
    void addFace(const std::vector<size_t>& idxs);
    void removeFace(const size_t& idx);

protected:
    virtual Vertex* allocateVertex();
    virtual Face* allocateFace();
    virtual void setDirty();

private:
    void copy(const Mesh& other);

protected:
    std::vector<Vertex*> m_Vertices;
    std::vector<Face*> m_Faces;
};

#endif //_MESH_MESH_H_
