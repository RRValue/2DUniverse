#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"

HESCutter::HESCutter() : m_SourceMesh(nullptr)
{

}

HESCutter::~HESCutter()
{

}

const CutPointVector& HESCutter::getCutPoints() const
{
    return m_CutPoints;
}

void HESCutter::clearCutPoints()
{
    for(auto p : m_CutPoints)
        delete p;

    m_CutPoints.clear();
}

bool HESCutter::cutLine(HESMesh* const sourceMesh, Line* const line, HESMeshVector& outMeshes)
{
    if(sourceMesh == nullptr || line == nullptr)
        return false;

    m_SourceMesh = sourceMesh;

    // reset
    clearCutPoints();
    
    // find cuts in border of mesh
    /// if we have no cutts -> the line cutts no parts of the mesh
    /// if we have a odd number of cutts -> the line cuts at one part of the mesh only one border of that part -> cut unfinished
    /// otherwise we have a cut

    std::vector<HESEdgeConstVector> boundaries;

    for(const auto& e : m_SourceMesh->getBoundaryStartEdges())
    {
        HESEdgeConstVector boundary;
        m_SourceMesh->walkBoundary(e, boundary);

        boundaries.push_back(boundary);
    }

    // if we have no boundaries -> a line can not cut a manifold without border -> return false
    if(boundaries.empty())
        return false;

    Line cut_edge;
    IntersectionVector cur_cut_points;
    IntersectionVector cut_points;
    std::vector<HESEdgeConstVector> border_cut_edges;

    for(const auto& b : boundaries)
    {
        HESEdgeConstVector cur_border_cut_edges;

        for(const auto& e : b)
        {
            cut_edge.setPoint(0, e->from()->getPosition());
            cut_edge.setPoint(1, e->to()->getPosition());

            cur_cut_points = cut_edge.intersect(line);

            if(cur_cut_points.empty())
                continue;

            for(const auto& ccp : cur_cut_points)
                cut_points.push_back(ccp);

            cur_border_cut_edges.push_back(e);
        }

        if(!cur_border_cut_edges.empty())
            border_cut_edges.push_back(cur_border_cut_edges);
    }

    // if no cuts -> we have no valid cut -> return false
    if(cut_points.empty())
        return false;

    // if number of cuts is odd -> we have no valid cut -> return false
    for(const auto& bce : border_cut_edges)
        if(bce.size() % 2 != 0)
            return false;

    return true;
}
