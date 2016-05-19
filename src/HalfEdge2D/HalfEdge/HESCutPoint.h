#ifndef _HALFEDGESTRUCTURE_CUTPOINT_H_
#define _HALFEDGESTRUCTURE_CUTPOINT_H_

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

    HESCutPoint& operator=(const HESCutPoint& other);

    bool hasSameVertex(const HESCutPoint& cutPoint) const;
    bool alignedOnBorder(const HESCutPoint& cutPoint) const;
    bool shareSameEdge(const HESCutPoint& cutPoint) const;

private:
    void checkOnVertex();

public:
    HESEdge* m_Edge;
    Vec2d m_Point;
    HESVertex* m_Vertex;
    bool m_IsOnVertex;
    bool m_IsOnBorder;

    const double m_VertexDistanceThreshold = 0.05;
};

typedef std::map<double, HESCutPoint, std::less<double>, Eigen::aligned_allocator<std::pair<double, HESCutPoint>>> HESCutMap;
typedef std::vector<HESCutPoint, Eigen::aligned_allocator<HESCutPoint>> HESCutVector;

#endif //_HALFEDGESTRUCTURE_CUTPOINT_H_