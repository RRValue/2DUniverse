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
    HESCutPoint(HESEdge* const e, const Vec2d& p, const bool& border, const double& alpha);
    HESCutPoint(const HESCutPoint& other);

    HESCutPoint& operator=(HESCutPoint& other);

    void snapToVertex();
    bool hasSameVertex(const HESCutPoint& cutPoint) const;
    bool cutsTriangle(const HESCutPoint& cutPoint) const;

public:
    HESEdge* m_Edge;
    Vec2d m_Point;
    HESVertex* m_Vertex;
    bool m_IsOnVertex;
    bool m_IsOnBorder;
    double m_Alpha;

    const double m_VertexDistanceThreshold = 0.05;
};

typedef std::map<double, HESCutPoint, std::less<double>, Eigen::aligned_allocator<std::pair<double, HESCutPoint>>> HESCutMap;
typedef std::vector<HESCutPoint, Eigen::aligned_allocator<HESCutPoint>> HESCutVector;

#endif //_HALFEDGESTRUCTURE_CUTPOINT_H_
