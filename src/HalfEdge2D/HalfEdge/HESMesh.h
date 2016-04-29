#ifndef _HALFEDGESTRUCTURE_MESH_H_
#define _HALFEDGESTRUCTURE_MESH_H_

#include "HalfEdge2D/Mesh/Mesh.h"

class HESVertex;
class HESFace;
class HESEdge;

class HESMesh : public Mesh
{
public:
    HESMesh();
    virtual ~HESMesh();

    HESMesh& operator=(const HESMesh& other);

    virtual void clear();

    // getter
    size_t getNumEdges() const;
    HESEdge* getHESEdge(const size_t& idx) const;
    HESVertex* getHESVertex(const size_t& idx) const;
    HESFace* getHESFace(const size_t& idx) const;
    const bool& hasChanged() const;

    // setter
    void addEdge(HESEdge* const edge);

protected:
    virtual Vertex* allocateVertex();
    virtual Face* allocateFace();

private:
    bool m_Changed;

    std::vector<HESEdge*> m_Edges;
};

#endif //_HALFEDGESTRUCTURE_MESH_H_
