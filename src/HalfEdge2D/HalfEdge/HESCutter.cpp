#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"
#include "HalfEdge2D/Renderables/QuadraticBezier.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"
#include "HalfEdge2D/Renderables/Spline.h"

#include <deque>

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

bool HESCutter::cutLine(HESMesh* const sourceMesh, Line* const line, HESMeshVector& outMeshes)
{
    if(sourceMesh == nullptr || line == nullptr)
        return false;

    m_SourceMesh = sourceMesh;

    // set line
    m_Line = line;
    m_ClosedCurve = false;

    // set mode
    m_CuttingMode = E_CM_LINE;

    return cut(outMeshes);
}

bool HESCutter::cutQuadraticBezier(HESMesh* const sourceMesh, QuadraticBezier* const qezier, HESMeshVector& outMeshes)
{
    if(sourceMesh == nullptr || qezier == nullptr)
        return false;

    m_SourceMesh = sourceMesh;

    // set line
    m_QuadraticBezier = qezier;
    m_ClosedCurve = false;

    // set mode
    m_CuttingMode = E_CM_QBEZIER;

    return cut(outMeshes);
}

bool HESCutter::cutCubicBezier(HESMesh* const sourceMesh, CubicBezier* const cezier, HESMeshVector& outMeshes)
{
    if(sourceMesh == nullptr || cezier == nullptr)
        return false;

    m_SourceMesh = sourceMesh;

    // set line
    m_CubicBezier = cezier;
    m_ClosedCurve = false;

    // set mode
    m_CuttingMode = E_CM_CBEZIER;

    return cut(outMeshes);
}

bool HESCutter::cutSpline(HESMesh* const sourceMesh, Spline* const spline, HESMeshVector& outMeshes)
{
    if(sourceMesh == nullptr || spline == nullptr)
        return false;

    m_SourceMesh = sourceMesh;

    // set line
    m_Spline = spline;
    m_ClosedCurve = m_Spline->isClosed();

    // set mode
    m_CuttingMode = E_CM_SPLINE;

    return cut(outMeshes);
}

bool HESCutter::cut(HESMeshVector& outMeshes)
{
    // reset
    m_CutPoints.clear();

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

    HESEdgeConstVector edges_visited;

    Line cut_edge;
    IntersectionVector cur_cut_points;
    IntersectionVector border_cut_points;
    std::vector<HESEdgeConstVector> border_cut_edges;

    CutPointMap cut_point_map;

    for(const auto& b : boundaries)
    {
        HESEdgeConstVector cur_border_cut_edges;

        for(const auto& e : b)
        {
            cut_edge.setPoint(0, e->from()->getPosition());
            cut_edge.setPoint(1, e->to()->getPosition());

            cur_cut_points = cutImpl(cut_edge);;

            if(cur_cut_points.empty())
                continue;

            for(const auto& ccp : cur_cut_points)
            {
                border_cut_points.push_back(ccp);

                cut_point_map.insert(std::make_pair(ccp.m_Alpha, CutPoint(e, ccp.m_Point)));
            }

            cur_border_cut_edges.push_back(e);

            e->setVisited(true);
            edges_visited.push_back(e);
        }

        if(!cur_border_cut_edges.empty())
            border_cut_edges.push_back(cur_border_cut_edges);
    }

    // if no cuts -> we have no valid cut -> return false
    if(border_cut_points.empty())
        return false;

    // if number of cuts on one border is odd -> we have no valid cut -> return false
    for(const auto& bce : border_cut_edges)
        if(bce.size() % 2 != 0)
            return false;

    // get cut points from edges
    HESFaceConstVector faces_visited;
    std::deque<HESFace* const> faces_to_visit;

    for(const auto& bce : border_cut_edges)
        for(const auto& e : bce)
            faces_to_visit.push_back(e->face());

    HESFace* current_face;

    while(!faces_to_visit.empty())
    {
        current_face = faces_to_visit.front();
        faces_to_visit.pop_front();

        if(current_face->visited())
            continue;

        current_face->setVisited(true);
        faces_visited.push_back(current_face);

        // cut with edges on face
        for(const auto& e : current_face->getEdges())
        {
            if(e->visited())
                continue;

            // add to visited
            e->setVisited(true);
            edges_visited.push_back(e);

            if(e->opposite() != nullptr)
            {
                e->opposite()->setVisited(true);
                edges_visited.push_back(e->opposite());
            }

            // try a cut with line
            cut_edge.setPoint(0, e->from()->getPosition());
            cut_edge.setPoint(1, e->to()->getPosition());

            cur_cut_points = cutImpl(cut_edge);

            if(cur_cut_points.empty())
                continue;

            faces_to_visit.push_back(e->opposite()->face());

            for(const auto& ccp : cur_cut_points)
                cut_point_map.insert(std::make_pair(ccp.m_Alpha, CutPoint(e, ccp.m_Point)));
        }
    }

    // set everythinf to unvisit
    for(const auto& e : edges_visited)
        e->setVisited(false);

    for(const auto& f : faces_visited)
        f->setVisited(false);

    // set cut points
    for(const auto& cp : cut_point_map)
        m_CutPoints.push_back(cp.second.m_Point);

    return true;
}

IntersectionVector HESCutter::cutImpl(const Line& line) const
{
    switch(m_CuttingMode)
    {
    case E_CM_LINE:
            return m_Line->intersect(const_cast<Line* const>(&line));
    case E_CM_QBEZIER:
        return m_QuadraticBezier->intersect(const_cast<Line* const>(&line));
    case E_CM_CBEZIER:
        return m_CubicBezier->intersect(const_cast<Line* const>(&line));
    case E_CM_SPLINE:
        return m_Spline->intersect(const_cast<Line* const>(&line));
    default:
        return IntersectionVector();
    }

    return IntersectionVector();
}
