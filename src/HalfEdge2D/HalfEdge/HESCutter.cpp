#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Renderables/Point.h"

HESCutter::HESCutter() : m_SourceMesh(nullptr)
{

}

HESCutter::HESCutter(HESMesh* const mesh) : m_SourceMesh(mesh)
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

bool HESCutter::cutLine(HESMesh* const outMesh, Line* const line)
{
    if(outMesh == nullptr)
        return false;

    if(m_SourceMesh == nullptr)
        return false;

    if(line == nullptr)
        return false;

    m_TargetMesh = outMesh;

    // reset
    clearCutPoints();

    // prepare target mesh
    prepareTargetMesh();
    
    // find cuts in border of mesh
    /// if we have no cutts -> the line cutts no parts of the mesh
    /// if we have a odd number of cutts -> the line cuts at one part of the mesh only one border of that part -> cut unfinished
    /// otherwise we have a cut

    return false;
}

void HESCutter::prepareTargetMesh()
{
    // copy everthing to from source to target mesh
    *m_TargetMesh = *m_SourceMesh;

    // find borders and assign null edges
    /// to do
}
