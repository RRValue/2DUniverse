#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

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

    // reset result and intermediate objects
    m_CutPoints.clear();
    m_CutPointMap.clear();
    m_CutPointVector.clear();
    m_BorderCutEdges.clear();
    m_BorderCuts.clear();

    // find cuts in border of mesh
    findMeshBoundaries();

    // if we have no boundaries -> a (opend or closed) line can not cut a manifold without border -> return false
    if(m_MeshBoundaries.empty())
        return false;

    // get all boundary cuts
    findBoundaryCuts();

    // if we have no border cuts
    if(m_BorderCuts.empty())
    {
        // if we have no closed curve -> no cutting possible
        if(!m_ClosedCurve)
            return false;
        else
        {
            findOneMeshCut();

            // if we still have cut -> we have no cut at all
            if(m_BorderCuts.empty())
                return false;
        }
    }

    // if number of cuts on one border (if any exists) is odd -> we have no valid cut -> return false
    for(const auto& bce : m_BorderCuts)
        if(bce % 2 != 0)
            return false;

    // get cut points
    findCutPoints();

    // merge same cut points
    mergeSameCutPoints();

    // set cut points
    for(const auto& cp : m_CutPointVector)
        m_CutPoints.push_back(cp.m_Point);

    return true;
}

void HESCutter::findMeshBoundaries()
{
    m_MeshBoundaries.clear();

    for(const auto& e : m_SourceMesh->getBoundaryStartEdges())
    {
        HESEdgeConstVector boundary;
        m_SourceMesh->walkBoundary(e, boundary);

        m_MeshBoundaries.push_back(boundary);
    }
}

void HESCutter::findBoundaryCuts()
{
    Line cut_edge;
    IntersectionVector cur_cut_points;
    IntersectionVector border_cut_points;

    for(const auto& b : m_MeshBoundaries)
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

                m_CutPointMap.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(e, ccp.m_Point)));
            }

            if(cur_cut_points.size() > 0)
                m_BorderCutEdges.push_back(e);

            cur_num_border_cuts += cur_cut_points.size();

            e->setVisited(true);
            m_VisitedEdges.push_back(e);
        }

        m_BorderCuts.push_back(cur_num_border_cuts);
    }
}

void HESCutter::findOneMeshCut()
{
    Line cut_edge;
    IntersectionVector cur_cut_points;

    // find at least one cut edge
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
            m_CutPointMap.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(current_edge, ccp.m_Point)));

        if(cur_cut_points.size() == 0)
            continue;

        m_BorderCutEdges.push_back(current_edge);

        break;
    }
}

void HESCutter::findCutPoints()
{
    Line cut_edge;
    IntersectionVector cur_cut_points;

    // get cut points from edges
    std::deque<HESFace* const> faces_to_visit;

    for(const auto& e : m_BorderCutEdges)
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
                m_CutPointMap.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(e, ccp.m_Point)));
        }
    }

    // transform to cut_point_map to vector
    size_t idx = 0;
    for(const auto& cp : m_CutPointMap)
        m_CutPointVector.push_back(cp.second);

    m_CutPointMap.clear();
}

void HESCutter::mergeSameCutPoints()
{
    // check if a cut point is on a vertex -> if yes move adjacent CutPoint to this point
    HESCutVector::iterator cut_iter = m_CutPointVector.begin();
    size_t cut_idx = 0;

    while(cut_iter != m_CutPointVector.end())
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
            if(group_iter_0 == m_CutPointVector.begin())
                break;

            group_iter_0--;
            cut_idx--;

            if(!cut_iter->hasSameNearestVertex(*group_iter_0))
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

            if(group_iter_1 == m_CutPointVector.end())
                break;

            if(!cut_iter->hasSameNearestVertex(*group_iter_1))
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
        m_CutPointVector.erase(group_iter_0, group_iter_1);

        cut_iter = m_CutPointVector.begin() + cut_idx;
    }
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
