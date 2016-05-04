#ifndef _HALFEDGESTRUCTURE_CHECK_H_
#define _HALFEDGESTRUCTURE_CHECK_H_

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include <vector>
#include <set>
#include <map>
#include <deque>

class HESCheck
{
public:
    enum HESCheckError
    {
        E_HESCE_FIRST,
        E_HESCE_OK,
        E_HESCE_BOUNDARYSEARCH,
        E_HESCE_SPLIT,
        E_HESCE_LAST,
    };

private:
    typedef std::vector<HESMesh*> HESMeshVector;
    typedef std::vector<HESFace* const> HESFaceVector;
    typedef std::deque<HESFace* const> HESFaceDeque;
    typedef std::vector<HESEdge* const> HESEdgeConstVector;
    typedef std::set<HESVertex* const> HESVertexConstSet;

    typedef std::map<HESFace*, HESEdgeConstVector> HESFaceBoundaryMap;
    typedef std::set<HESFace* const> HESFaceSet;

    struct MeshPart
    {
        MeshPart()
        {
            m_NumHoles = 0;
        }

        HESFaceVector m_Faces;
        size_t m_NumHoles;
    };

    typedef std::vector<MeshPart> HESMeshPartVector;

public:
    HESCheck() = delete;
    HESCheck(HESMesh* const mesh);
    ~HESCheck();

private:
    void run();

    void hasPartsConnectedInOneVertex();
    void splitPartsConnectedInOneVertex();

    void findBoundaries();
    HESEdgeConstVector walkBoundary(HESEdge* const edge);

    void findParts();

    void createMeshesFromParts();

private:
    HESMesh* const m_SourceMesh;
    HESMesh* m_ProcessingMesh;

    HESCheckError m_Error;

    HESVertexConstSet m_PartsConnectingVertices;
    HESFaceBoundaryMap m_Boundaries;
    HESMeshPartVector m_MeshParts;

    bool m_HasPartConnectedInOneVertex;
    bool m_HasParts;
    bool m_HasHoles;
};

#endif //_HALFEDGESTRUCTURE_CHECK_H_
