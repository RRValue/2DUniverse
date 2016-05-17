#ifndef _HALFEDGESTRUCTURE_MESH_H_
#define _HALFEDGESTRUCTURE_MESH_H_

#include "HalfEdge2D/Mesh/Mesh.h"

#include <set>

class HESVertex;
class HESFace;
class HESEdge;

class HESBuilder;
class HESCheck;

typedef std::vector<HESEdge* const> HESEdgeConstVector;
typedef std::vector<HESFace* const> HESFaceConstVector;

class HESMesh : public Mesh
{
    friend class HESBuilder;
    friend class HESCheck;

public:
    HESMesh();
    HESMesh(const HESMesh& other);
    virtual ~HESMesh();

    HESMesh& operator=(const HESMesh& other);

    virtual void clear();

    // getter
    size_t getNumEdges() const;
    HESEdge* getHESEdge(const size_t& idx) const;
    HESVertex* getHESVertex(const size_t& idx) const;
    HESFace* getHESFace(const size_t& idx) const;
    const std::vector<HESEdge*>& getEdges() const;
    const bool& isChecked() const;
    bool isManifold() const;
    const bool& isValid() const;
    size_t getNumBoundaries() const;
    std::set<HESEdge*> getBoundaryStartEdges() const;

    // tooling
    bool walkBoundary(HESEdge* const edge, HESEdgeConstVector& boundary);
    HESVertex* const splitEdgeAtPoint(HESEdge* const splitEdge, const Vec2d& splitPoint);

protected:
    // setter
    void addEdge(HESEdge* const edge);
    void addBoundaryStartEdge(HESEdge* const edge);
    void setChecked(const bool& checked);
    void setValid(const bool& valid);

    // tooling
    HESEdge* const insertVertexOnEdge(HESVertex* vertex, const size_t& vertexId, HESEdge* const edge);

protected:
    virtual Vertex* allocateVertex();
    virtual Face* allocateFace();
    virtual void setDirty();

private:
    bool m_Checked;
    bool m_Valid;

    std::vector<HESEdge*> m_Edges;
    std::set<HESEdge*> m_BoundaryStartEdges;
};

#endif //_HALFEDGESTRUCTURE_MESH_H_
