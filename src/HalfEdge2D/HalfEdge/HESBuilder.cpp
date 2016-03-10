#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/Mesh/Triangle.h"
#include "HalfEdge2D/HalfEdge/HESVertex.h"

HESBuilder::HESBuilder(HESMesh* const mesh) : m_Mesh(mesh)
{

}

HESBuilder::~HESBuilder()
{

}

bool HESBuilder::build()
{
    if(m_Mesh == nullptr)
        return false;

    // iterate over all Triangles and build basis triangles
    for(const auto& tris : m_Mesh->getTriangles())
    {

    }

    return true;
}
