#ifndef _HALFEDGESTRUCTURE_MESH_H_
#define _HALFEDGESTRUCTURE_MESH_H_

#include "HalfEdge2D/Mesh/Mesh.h"

class HESVertex;

class HESMesh : public Mesh
{
public:
    HESMesh();
    virtual ~HESMesh();

    // getter
    HESVertex* getHESVertix(const size_t& idx) const;
    const bool& hasChanged() const;

protected:
    virtual Vertex* allocateVertex();

private:
    bool m_Changed;
};

#endif //_HALFEDGESTRUCTURE_MESH_H_
