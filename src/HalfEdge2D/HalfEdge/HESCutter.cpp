#include "HalfEdge2D/HalfEdge/HESCutter.h"

#include "HalfEdge2D/Mesh/Face.h"

#include "HalfEdge2D/HalfEdge/HESVertex.h"
#include "HalfEdge2D/HalfEdge/HESEdge.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

HESCutter::HESCutter(HESMesh* const mesh) : m_SourceMesh(mesh)
{

}

HESCutter::~HESCutter()
{

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

    // start cut here


    return true;
}
