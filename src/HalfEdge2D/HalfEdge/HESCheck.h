#ifndef _HALFEDGESTRUCTURE_CHECK_H_
#define _HALFEDGESTRUCTURE_CHECK_H_

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include <vector>
#include <set>

class HESCheck
{
private:
    typedef std::vector<HESMesh*> HESMeshVector;
    typedef std::vector<HESEdge* const> HESEdgeConstVector;
    typedef std::set<HESVertex* const> HESVertexConstSet;

public:
    HESCheck() = delete;
    HESCheck(HESMesh* const mesh);
    ~HESCheck();

private:
    void run();

    void checkHasPartsConnectedInOneVertex();
    void splitPartsConnectedInOneVertex();

    void splitParts();

    HESEdgeConstVector findBoundary(HESEdge* const edge);

private:
    HESMesh* const m_SourceMesh;
    HESMesh* m_ProcessingMesh;
    HESMeshVector m_MeshParts;

    HESVertexConstSet m_PartsConnectingVertices;

    bool m_HasPartConnectedInOneVertex;
    bool m_HasParts;
};

#endif //_HALFEDGESTRUCTURE_CHECK_H_
