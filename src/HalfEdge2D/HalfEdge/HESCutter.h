#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Renderables/Intersectable.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESCutPoint.h"

#include <vector>
#include <map>

class HESEdge;
class HESVertex;
class HESCutPoint;

class Line;
class QuadraticBezier;
class CubicBezier;
class Spline;
class Point;

typedef std::vector<Vec2d, Eigen::aligned_allocator<Vec2d>> CutPointVector;
typedef std::vector<CutPointVector> PointCutsVector;
typedef std::vector<HESMesh*> HESMeshVector;

class HESCutter
{
private:
    class HESCutterCleanGuard
    {
    public:
        HESCutterCleanGuard(HESCutter* const cutter);
        ~HESCutterCleanGuard();

        void* operator new(size_t size) = delete;
        void* operator new[](size_t size) = delete;
        void operator delete(void* ptr) = delete;
        void operator delete[](void* ptr)  = delete;

    private:
        HESCutter* const m_Cutter;
    };

private:
    enum CuttingMode
    {
        E_CM_FIRST,
        E_CM_LINE,
        E_CM_QBEZIER,
        E_CM_CBEZIER,
        E_CM_SPLINE,
        E_CM_LAST,
    };

public:
    HESCutter();
    ~HESCutter();

    const PointCutsVector& getPointCuts() const;

    bool cutLine(HESMesh* const sourceMesh, Line* const line, HESMeshVector& outMeshes);
    bool cutQuadraticBezier(HESMesh* const sourceMesh, QuadraticBezier* const qezier, HESMeshVector& outMeshes);
    bool cutCubicBezier(HESMesh* const sourceMesh, CubicBezier* const cezier, HESMeshVector& outMeshes);
    bool cutSpline(HESMesh* const sourceMesh, Spline* const spline, HESMeshVector& outMeshes);

    void enableSmallTrianglePrevention(const bool& enable);

private:
    bool cut(HESMeshVector& outMeshes);
    bool cut(HESMeshVector& outMeshes, HESMesh* const mesh);

    IntersectionVector cutImpl(const Line& line) const;

    void findMeshBoundaries(HESMesh* const mesh);
    void findBoundaryCuts();
    void findOneMeshCut(HESMesh* const mesh);
    void findCutPoints();
    void makeCutLines();
    void snapCutLines();
    void cleanUpSnapedCutLines();
    void createCutVertices(HESMesh* const mesh);

private:
    // meshes
    HESMesh* m_SourceMesh;
    HESMeshVector m_TargetMeshes;

    // mode
    CuttingMode m_CuttingMode;

    // cut objects and properties
    Line* m_Line;
    QuadraticBezier* m_QuadraticBezier;
    CubicBezier* m_CubicBezier;
    Spline* m_Spline;

    bool m_ClosedCurve;
    bool m_SmallTrianglePrevention;

    // visited
    HESEdgeConstVector m_VisitedEdges;
    HESFaceConstVector m_VisitedFaces;

    PointCutsVector m_PointCuts;

    // intermediate objects
    std::vector<HESEdgeConstVector> m_MeshBoundaries;
    std::vector<size_t> m_BorderCuts;
    HESEdgeConstVector m_BorderCutEdges;
    HESCutMap m_CutPointMap;
    HESCutVector m_CutPointVector;
    std::vector<HESCutVector> m_CutLines;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
