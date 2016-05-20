#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"
#include "HalfEdge2D/HalfEdge/HESCheck.h"

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

    // delete processing mesh
    for(const auto& m : m_Cutter->m_TargetMeshes)
        delete m;

    m_Cutter->m_TargetMeshes.clear();
}

HESCutter::HESCutter() : m_SourceMesh(nullptr)
{

}

HESCutter::~HESCutter()
{

}

const PointCutsVector& HESCutter::getPointCuts() const
{
    return m_PointCuts;
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

    // setup target meshes
    HESCheck source_mesh_checker;
    source_mesh_checker.run(m_SourceMesh);

    m_TargetMeshes = source_mesh_checker.getMeshes();

    for(const auto& m : m_TargetMeshes)
    {
        // reset result and intermediate objects
        m_MeshBoundaries.clear();
        m_PointCuts.clear();
        m_CutPointMap.clear();
        m_CutPointVector.clear();
        m_CutLines.clear();
        m_BorderCutEdges.clear();
        m_BorderCuts.clear();

        // cut current mesh
        HESMeshVector current_cut_meshes;

        bool cutted = cut(current_cut_meshes, m);

        if(!cutted)
        {
            outMeshes.clear();

            return false;
        }

        outMeshes.insert(outMeshes.end(), current_cut_meshes.begin(), current_cut_meshes.end());
    }

    return true;
}

bool HESCutter::cut(HESMeshVector& outMeshes, HESMesh* const mesh)
{
    // find cuts in border of mesh
    findMeshBoundaries(mesh);

    // if we have no boundaries -> a (opend or closed) line can not cut a manifold without border -> return false
    if(m_MeshBoundaries.empty())
        return false;

    // get all boundary cuts
    findBoundaryCuts();

    // if we have no border cuts
    if(m_BorderCutEdges.empty())
    {
        // if we have no closed curve -> no cutting possible
        if(!m_ClosedCurve)
            return false;
        else
        {
            findOneMeshCut(mesh);

            // if we still have no cut -> we have no cut at all
            if(m_BorderCutEdges.empty())
                return false;
        }
    }

    // if number of cuts on one border (if any exists) is odd -> we have no valid cut -> return false
    for(const auto& bce : m_BorderCuts)
        if(bce % 2 != 0)
            return false;

    // get cut points
    findCutPoints();

    // seperate point cuts
    makeCutLines();

    // merge same cut points
    mergeSameCutPoints();

    // set cut points
    for(const auto& cpv : m_CutLines)
    {
        CutPointVector cur_cps;

        for(const auto& cp : cpv)
            cur_cps.push_back(cp.m_Point);

        m_PointCuts.push_back(cur_cps);
    }

    // create cut vertices
    /*createCutVertices(mesh);

    HESCheck checker;
    checker.run(mesh);

    outMeshes = checker.getMeshes();*/

    return true;
}

void HESCutter::findMeshBoundaries(HESMesh* const mesh)
{
    for(const auto& e : mesh->getBoundaryStartEdges())
    {
        HESEdgeConstVector boundary;
        mesh->walkBoundary(e, boundary);

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

                m_CutPointMap.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(e, ccp.m_Point, true, ccp.m_Alpha)));
            }

            if(cur_cut_points.size() > 0)
                m_BorderCutEdges.push_back(e);

            cur_num_border_cuts += cur_cut_points.size();

            e->setVisited(true);
            m_VisitedEdges.push_back(e);
        }
        
        if(cur_num_border_cuts > 0)
            m_BorderCuts.push_back(cur_num_border_cuts);
    }
}

void HESCutter::findOneMeshCut(HESMesh* const mesh)
{
    Line cut_edge;
    IntersectionVector cur_cut_points;

    // find at least one cut edge
    size_t num_mesh_edges = mesh->getNumEdges();
    HESEdge* current_edge;
    for(size_t i = 0; i < num_mesh_edges; i++)
    {
        current_edge = mesh->getHESEdge(i);

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
            m_CutPointMap.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(current_edge, ccp.m_Point, false, ccp.m_Alpha)));

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
                m_CutPointMap.insert(std::make_pair(ccp.m_Alpha, HESCutPoint(e, ccp.m_Point, false, ccp.m_Alpha)));
        }
    }

    // transform to cut_point_map to vector
    size_t idx = 0;
    for(const auto& cp : m_CutPointMap)
        m_CutPointVector.push_back(cp.second);

    m_CutPointMap.clear();
}

void HESCutter::makeCutLines()
{
    if(m_CutPointVector.empty())
        return;

    if(m_BorderCuts.empty())
    {
        m_CutLines.push_back(m_CutPointVector);
        m_CutPointVector.clear();

        return;
    }

    // set variables
    HESCutVector tmp_cut_vec;

    HESCutVector::const_iterator i0;
    HESCutVector::const_iterator i1;
    HESCutVector::const_iterator i2;

    // if we start with non border cuts move them to the end of the vector
    // e.g. cccbbbbcccbbcc -> bcccbbbbcccccb
    i0 = m_CutPointVector.begin();

    if(!i0->m_IsOnBorder)
    {
        i1 = i0 + 1;

        while(!i1->m_IsOnBorder)
            i1++;

        i1++;

        tmp_cut_vec.insert(tmp_cut_vec.begin(), i0, i1);
        m_CutPointVector.erase(i0, i1);
        m_CutPointVector.insert(m_CutPointVector.end(), tmp_cut_vec.begin(), tmp_cut_vec.end());
        tmp_cut_vec.clear();
    }

    // check cut triangle
    auto border_points_cuts_triangle = [](const HESCutPoint& p0, const HESCutPoint& p1)
    {
        return p0.m_Edge != p1.m_Edge && p0.m_Edge->face() == p1.m_Edge->face();
    };

    // find first two cut points which make a true cut
    i0 = m_CutPointVector.begin();
    i1 = i0;
    i2 = i1 + 1;

    while(true)
    {
        if(i2 == m_CutPointVector.end())
            break;

        if(i1->m_IsOnBorder && i2->m_IsOnBorder && !border_points_cuts_triangle(*i1, *i2))
        {
            i1++;
            i2++;
        }
        else
        {
            tmp_cut_vec.insert(tmp_cut_vec.begin(), i0, i1);
            m_CutPointVector.erase(i0, i1);
            m_CutPointVector.insert(m_CutPointVector.end(), tmp_cut_vec.begin(), tmp_cut_vec.end());
            tmp_cut_vec.clear();

            break;
        }
    }

    // separate
    i0 = m_CutPointVector.begin();

    while(i0 != m_CutPointVector.end())
    {
        i1 = i0 + 1;

        while(i1 == m_CutPointVector.end() || !i1->m_IsOnBorder)
            i1++;

        if(i1 != m_CutPointVector.end())
            i1++;

        tmp_cut_vec.insert(tmp_cut_vec.begin(), i0, i1);
        m_CutPointVector.erase(i0, i1);
        m_CutLines.push_back(tmp_cut_vec);
        tmp_cut_vec.clear();

        i0 = m_CutPointVector.begin();
    }

    // check on seperated cuts if two cuts are on the same edge (if yes remove)
    std::vector<HESCutVector>::const_iterator cvi0 = m_CutLines.begin();
    size_t idx = 0;

    while(cvi0 != m_CutLines.end())
    {
        if(cvi0->size() == 2 && !border_points_cuts_triangle((*cvi0)[0], (*cvi0)[1]))
        {
            m_CutLines.erase(cvi0);
            cvi0 = m_CutLines.begin() + idx;
        }
        else
        {
            cvi0++;
            idx++;
        }
    }
}

void HESCutter::mergeSameCutPoints()
{
    for(auto& cv : m_CutLines)
    {
        // check if a cut point is on a vertex -> if yes move adjacent CutPoint to this point
        HESCutVector::iterator cut_iter = cv.begin();
        size_t cut_idx = 0;

        while(cut_iter != cv.end())
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
                if(group_iter_0 == cv.begin())
                    break;

                group_iter_0--;
                cut_idx--;

                if(!cut_iter->hasSameVertex(*group_iter_0))
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

                if(group_iter_1 == cv.end())
                    break;

                if(!cut_iter->hasSameVertex(*group_iter_1))
                    break;
            }

            // delete everthing exept the first one
            if(group_iter_0 != cut_iter)
            {
                *group_iter_0 = *cut_iter;

                group_iter_0->m_Point = cut_iter->m_Vertex->getPosition();
            }
            else
                group_iter_0->m_Point = group_iter_0->m_Vertex->getPosition();

            group_iter_0++;
            cut_idx++;
            cv.erase(group_iter_0, group_iter_1);

            cut_iter = cv.begin() + cut_idx;
        }
    }

    // dlean up cut lines
    cleanUpCutLines();
}

void HESCutter::cleanUpCutLines()
{
    // check if element in cutline is on border
    for(auto& cl : m_CutLines)
    {
        for(auto& cp : cl)
        {
            cp.m_IsOnBorder = false;

            if(cp.m_IsOnVertex)
            {
                for(const auto& e : cp.m_Vertex->getEdges())
                {
                    if(e->opposite() != nullptr)
                        continue;

                    cp.m_IsOnBorder = true;

                    break;
                }
            }
            else
                cp.m_IsOnBorder = cp.m_Edge->opposite() == nullptr;
        }
    }

    // remove border cut points on start and end
    for(auto& cl : m_CutLines)
    {
        if(cl.size() <= 2)
            continue;

        // remove unnecessary borderpoints on start
        HESCutVector::const_iterator i0 = cl.begin();
        HESCutVector::const_iterator i1 = i0 + 1;

        if(i0->m_IsOnBorder)
        {
            while(true)
            {
                if(i1 == cl.end() || !i1->m_IsOnBorder)
                    break;

                i1++;
            }

            i1--;

            cl.erase(i0, i1);
        }

        if(cl.size() <= 2)
            continue;

        // remove unnecessary borderpoints on end
        i1 = cl.end() - 1;
        i0 = i1 - 1;

        if(i1->m_IsOnBorder)
        {
            while(true)
            {
                if(!i0->m_IsOnBorder || i0 == cl.begin())
                    break;

                i0--;
            }

            if(!i0->m_IsOnBorder)
                i0++;

            i0++;
            i1++;

            cl.erase(i0, i1);
        }
    }

    // remove cut lines with less than 2 elements or with 2 elements which are on a border
    std::vector<HESCutVector>::const_iterator cv_iter = m_CutLines.begin();
    size_t idx = 0;

    while(cv_iter != m_CutLines.end())
    {
        if(cv_iter->size() <= 1)
        {
            m_CutLines.erase(cv_iter);
            cv_iter = m_CutLines.begin() + idx;

            continue;
        }
        if(cv_iter->size() == 2 && (*cv_iter)[0].m_IsOnBorder || (*cv_iter)[1].m_IsOnBorder)
        {
            m_CutLines.erase(cv_iter);
            cv_iter = m_CutLines.begin() + idx;

            continue;
        }

        cv_iter++;
        idx++;
    }
}

void HESCutter::createCutVertices(HESMesh* const mesh)
{
    for(auto& cv : m_CutLines)
    {
        for(auto& cp : cv)
        {
            if(cp.m_IsOnVertex)
                continue;

            HESVertex* const new_vertex = mesh->splitEdgeAtPoint(cp.m_Edge, cp.m_Point);

            cp.m_IsOnVertex = true;
            cp.m_Vertex = new_vertex;
            cp.m_Edge = nullptr;
        }
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
