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
    const bool& hasChanged() const;
    const bool& isChecked() const;
    bool isManifold() const;
    const bool& isValid() const;
    size_t getNumBoundaries() const;
    std::set<HESEdge*> getBoundaryStartEdges() const;

    // tooling
    bool walkBoundary(HESEdge* const edge, HESEdgeConstVector& boundary);

protected:
    // setter
    void addEdge(HESEdge* const edge);
    void addBoundaryStartEdge(HESEdge* const edge);
    void setChecked(const bool& checked);
    void setValid(const bool& valid);

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
