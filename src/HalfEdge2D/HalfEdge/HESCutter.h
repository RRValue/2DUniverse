#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include "HalfEdge2D/Base/Vector.h"

#include <vector>
#include <map>

class HESMesh;
class HESEdge;
class Line;
class Point;

typedef std::vector<Vec2f, Eigen::aligned_allocator<Vec2f>> CutPointVector;
typedef std::vector<HESMesh*> HESMeshVector;

class HESCutter
{
private:
    struct CutPoint
    {
        CutPoint() = delete;
        CutPoint(HESEdge* const e, const Vec2f& p)
        {
            m_Edge = e;
            m_Point = p;
        }

        HESEdge* m_Edge;
        Vec2f m_Point;
    };

    typedef std::map<float, CutPoint, std::less<float>, Eigen::aligned_allocator<std::pair<float, CutPoint>>> CutPointMap;

public:
    HESCutter();
    ~HESCutter();

    const CutPointVector& getCutPoints() const;

    bool cutLine(HESMesh* const sourceMesh, Line* const line, HESMeshVector& outMeshes);

private:
    HESMesh* m_SourceMesh;
    HESMesh* m_TargetMesh;

    CutPointVector m_CutPoints;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
