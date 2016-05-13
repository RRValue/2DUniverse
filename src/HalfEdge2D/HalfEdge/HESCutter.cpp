#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESCutPoint.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"
#include "HalfEdge2D/Renderables/QuadraticBezier.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"
#include "HalfEdge2D/Renderables/Spline.h"

#include <deque>

HESCutter::HESCutterCleanGuard::HESCutterCleanGuard(HESCutter* const cutter) : m_Cutter(cutter)
{

}

HESCutter::HESCutterCleanGuard::~HESCutterCleanGuard()
{
    if(m_Cutter == nullptr)
        return;

    // set everything to unvisit
    for(const auto& e : m_Cutter->m_VisitedEdges)
        e->setVisited(false);

    m_Cutter->m_VisitedEdges.clear();

    for(const auto& f : m_Cutter->m_VisitedFaces)
        f->setVisited(false);

    m_Cutter->m_VisitedFaces.clear();
}

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
    // set clean up guard
    HESCutterCleanGuard clean_guard(this);

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

    Line cut_edge;
    IntersectionVector cur_cut_points;
    IntersectionVector border_cut_points;
    std::vector<size_t> border_cuts;
    HESEdgeConstVector border_cut_edges;

    HESCutMap cut_point_map;

    for(const auto& b : boundaries)
    {
        size_t cur_num_border_cuts = 0;

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

                cut_point_map.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(e, ccp.m_Point)));
            }

            if(cur_cut_points.size() > 0)
                border_cut_edges.push_back(e);

            cur_num_border_cuts += cur_cut_points.size();

            e->setVisited(true);
            m_VisitedEdges.push_back(e);
        }
        
        border_cuts.push_back(cur_num_border_cuts);
    }

    // check if border cut edges is empty
    if(border_cut_edges.empty())
    {
        // if no cuts with a non closed curve-> we have no valid cut -> return false
        if(!m_ClosedCurve)
            return false;

        // find at least one edge
        size_t num_mesh_edges = m_SourceMesh->getNumEdges();
        HESEdge* current_edge;
        for(size_t i = 0; i < num_mesh_edges; i++)
        {
            current_edge = m_SourceMesh->getHESEdge(i);

            if(current_edge->visited())
                continue;

            current_edge->setVisited(true);
            m_VisitedEdges.push_back(current_edge);

            if(current_edge->opposite() != nullptr)
            {
                current_edge->opposite()->setVisited(true);
                m_VisitedEdges.push_back(current_edge->opposite());
            }

            cut_edge.setPoint(0, current_edge->from()->getPosition());
            cut_edge.setPoint(1, current_edge->to()->getPosition());

            cur_cut_points = cutImpl(cut_edge);;

            if(cur_cut_points.empty())
                continue;

            for(const auto& ccp : cur_cut_points)
                cut_point_map.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(current_edge, ccp.m_Point)));

            if(cur_cut_points.size() == 0)
                continue;
            
            border_cut_edges.push_back(current_edge);

            break;
        }

        // if we still have no edge -> we have no cut
        if(border_cut_edges.empty())
            return false;
    }

    // if number of cuts on one border is odd -> we have no valid cut -> return false
    for(const auto& bce : border_cuts)
        if(bce % 2 != 0)
            return false;

    // get cut points from edges
    std::deque<HESFace* const> faces_to_visit;

    for(const auto& e : border_cut_edges)
    {
        faces_to_visit.push_back(e->face());

        if(e->opposite() != nullptr)
            faces_to_visit.push_back(e->opposite()->face());
    }

    HESFace* current_face;

    while(!faces_to_visit.empty())
    {
        current_face = faces_to_visit.front();
        faces_to_visit.pop_front();

        if(current_face->visited())
            continue;

        current_face->setVisited(true);
        m_VisitedFaces.push_back(current_face);

        // cut with edges on face
        for(const auto& e : current_face->getEdges())
        {
            if(e->visited())
                continue;

            // add to visited
            e->setVisited(true);
            m_VisitedEdges.push_back(e);

            if(e->opposite() != nullptr)
            {
                e->opposite()->setVisited(true);
                m_VisitedEdges.push_back(e->opposite());
            }

            // try a cut with line
            cut_edge.setPoint(0, e->from()->getPosition());
            cut_edge.setPoint(1, e->to()->getPosition());

            cur_cut_points = cutImpl(cut_edge);

            if(cur_cut_points.empty())
                continue;

            faces_to_visit.push_back(e->opposite()->face());

            for(const auto& ccp : cur_cut_points)
                cut_point_map.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(e, ccp.m_Point)));
        }
    }

    // transform to cut_point_map to vector
    HESCutVector cut_point_vector;
    size_t idx = 0;
    for(const auto& cp : cut_point_map)
        cut_point_vector.push_back(cp.second);

    cut_point_map.clear();

    // check if a cut point is on a vertex -> if yes move adjacent CutPoint to this point
    HESCutVector::iterator cut_iter = cut_point_vector.begin();
    size_t cut_idx = 0;

    while(cut_iter != cut_point_vector.end())
    {
        if(!cut_iter->m_IsOnVertex)
        {
            cut_iter++;
            cut_idx++;

            continue;
        }

        HESCutVector::iterator group_iter_0 = cut_iter;
        HESCutVector::iterator group_iter_1 = cut_iter;

        // search for start group iter
        while(true)
        {
            if(group_iter_0 == cut_point_vector.begin())
                break;

            group_iter_0--;
            cut_idx--;

            if(!canSnapToCut(&*cut_iter, &*group_iter_0))
            {
                group_iter_0++;
                cut_idx++;

                break;
            }
        }

        // search for end group iter
        while(true)
        {
            group_iter_1++;

            if(group_iter_1 == cut_point_vector.end())
                break;

            if(!canSnapToCut(&*cut_iter, &*group_iter_1))
                break;
        }

        // delete everthing exept the first one
        if(group_iter_0 != cut_iter)
        {
            *group_iter_0 = *cut_iter;

            group_iter_0->m_Point = cut_iter->m_NearestVertex->getPosition();
        }
        else
            group_iter_0->m_Point = group_iter_0->m_NearestVertex->getPosition();

        group_iter_0++;
        cut_idx++;
        cut_point_vector.erase(group_iter_0, group_iter_1);

        cut_iter = cut_point_vector.begin() + cut_idx;
    }

    // set cut points
    for(const auto& cp : cut_point_vector)
        m_CutPoints.push_back(cp.m_Point);

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

bool HESCutter::canSnapToCut(HESCutPoint* const reverenceCut, HESCutPoint* const cut)
{
    // if we dont share a vertex -> no snapping
    if(reverenceCut->m_NearestVertex != cut->m_NearestVertex)
        return false;

    return true;
}
