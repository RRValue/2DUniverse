#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Renderables/Intersectable.h"

#include <vector>
#include <map>

class HESMesh;
class HESEdge;
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
    enum CuttingMode
    {
        E_CM_FIRST,
        E_CM_LINE,
        E_CM_QBEZIER,
        E_CM_CBEZIER,
        E_CM_SPLINE,
        E_CM_LAST,
    };

private:
    struct CutPoint
    {
        CutPoint() = delete;
        CutPoint(HESEdge* const e, const Vec2d& p)
        {
            m_Edge = e;
            m_Point = p;
        }

        HESEdge* m_Edge;
        Vec2d m_Point;
    };

    typedef std::map<double, CutPoint, std::less<double>, Eigen::aligned_allocator<std::pair<double, CutPoint>>> CutPointMap;

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

private:
    HESMesh* m_SourceMesh;
    HESMesh* m_TargetMesh;

    CuttingMode m_CuttingMode;

    Line* m_Line;
    QuadraticBezier* m_QuadraticBezier;
    CubicBezier* m_CubicBezier;
    Spline* m_Spline;

    bool m_ClosedCurve;

    CutPointVector m_CutPoints;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
