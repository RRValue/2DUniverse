#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Renderables/Intersectable.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

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

    const CutPointVector& getCutPoints() const;

    bool cutLine(HESMesh* const sourceMesh, Line* const line, HESMeshVector& outMeshes);
    bool cutQuadraticBezier(HESMesh* const sourceMesh, QuadraticBezier* const qezier, HESMeshVector& outMeshes);
    bool cutCubicBezier(HESMesh* const sourceMesh, CubicBezier* const cezier, HESMeshVector& outMeshes);
    bool cutSpline(HESMesh* const sourceMesh, Spline* const spline, HESMeshVector& outMeshes);

private:
    bool cut(HESMeshVector& outMeshes);

    IntersectionVector cutImpl(const Line& line) const;

    bool canSnapToCut(HESCutPoint* const reverenceCut, HESCutPoint* const cut);

private:
    HESMesh* m_SourceMesh;
    HESMesh* m_TargetMesh;

    CuttingMode m_CuttingMode;

    Line* m_Line;
    QuadraticBezier* m_QuadraticBezier;
    CubicBezier* m_CubicBezier;
    Spline* m_Spline;

    // visited
    HESEdgeConstVector m_VisitedEdges;
    HESFaceConstVector m_VisitedFaces;

    bool m_ClosedCurve;

    CutPointVector m_CutPoints;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
