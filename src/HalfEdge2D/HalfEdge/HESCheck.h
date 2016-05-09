#ifndef _HALFEDGESTRUCTURE_CHECK_H_
#define _HALFEDGESTRUCTURE_CHECK_H_

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include <vector>
#include <set>
#include <map>
#include <deque>

typedef std::vector<HESEdge* const> HESEdgeConstVector;

class HESCheck
{
public:
    enum HESCheckError
    {
        E_HESCE_FIRST,
        E_HESCE_NOMESH,
        E_HESCE_OK,
        E_HESCE_BOUNDARYSEARCH,
        E_HESCE_SPLIT,
        E_HESCE_LAST,
    };

private:
    typedef std::vector<HESMesh*> HESMeshVector;
    typedef std::vector<HESFace* const> HESFaceVector;
    typedef std::deque<HESFace* const> HESFaceDeque;
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

    struct ReIndexVertex
    {
        ReIndexVertex()
        {
            m_Index = 0;
            m_Add = false;
            m_Part = 0;
        }

        ReIndexVertex(HESVertex* const vertex)
        {
            m_Vertex = vertex;
            m_Index = 0;
            m_Add = false;
            m_Part = 0;
        }

        ReIndexVertex& operator=(const ReIndexVertex& other)
        {
            m_Vertex = other.m_Vertex;
            m_Index = other.m_Index;
            m_Add = other.m_Add;
            m_Part = other.m_Part;
            
            return *this;
        }

        HESVertex* m_Vertex;
        size_t m_Index;
        bool m_Add;
        size_t m_Part;
    };

    typedef std::vector<MeshPart> HESMeshPartVector;
    typedef std::vector<ReIndexVertex, Eigen::aligned_allocator<ReIndexVertex>> ReIndexVector;

public:
    HESCheck();
    HESCheck(HESMesh* const mesh);
    ~HESCheck();

    const HESCheckError& getError() const;
    void run(HESMesh* const mesh);

    HESMeshVector getMeshes();

private:
    void run();
    void clear();
    void reset();

    void hasPartsConnectedInOneVertex();
    void splitPartsConnectedInOneVertex();

    void findBoundaries();

    void findParts();

    void createMeshesFromParts();

private:
    HESMesh* m_SourceMesh;
    HESMesh* m_ProcessingMesh;

    HESCheckError m_Error;

    HESVertexConstSet m_PartsConnectingVertices;
    HESFaceBoundaryMap m_Boundaries;
    HESMeshPartVector m_MeshParts;
    HESMeshVector m_Meshes;

    bool m_HasPartConnectedInOneVertex;
    bool m_HasParts;
    bool m_HasHoles;
};

#endif //_HALFEDGESTRUCTURE_CHECK_H_
