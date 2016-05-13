#ifndef _HALFEDGESTRUCTURE_CUT_H_
#define _HALFEDGESTRUCTURE_CUT_H_

#include "HalfEdge2D/Base/Vector.h"

#include <vector>
#include <map>

class HESEdge;
class HESVertex;

class HESCutPoint
{
public:
    HESCutPoint() = delete;
    HESCutPoint(HESEdge* const e, const Vec2d& p);

private:
    void checkOnVertex();

public:
    HESEdge* m_Edge;
    Vec2d m_Point;
    HESVertex* m_NearestVertex;
    bool m_IsOnVertex;

    const double m_VertexDistanceThreshold = 0.001f;
};

typedef std::map<double, HESCutPoint, std::less<double>, Eigen::aligned_allocator<std::pair<double, HESCutPoint>>> HESCutMap;
typedef std::vector<HESCutPoint, Eigen::aligned_allocator<HESCutPoint>> HESCutVector;

#endif //_HALFEDGESTRUCTURE_CUT_H_
