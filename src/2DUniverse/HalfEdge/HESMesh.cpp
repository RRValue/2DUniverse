#include "2DUniverse/HalfEdge/HESMesh.h"

#include "2DUniverse/Renderables/Renderable.h"

#include "2DUniverse/Mesh/Vertex.h"

#include "2DUniverse/HalfEdge/HESVertex.h"
#include "2DUniverse/HalfEdge/HESFace.h"
#include "2DUniverse/HalfEdge/HESEdge.h"
#include "2DUniverse/HalfEdge/HESBuilder.h"

HESMesh::HESMesh()
{
    m_Checked = false;
    m_Valid = true;
}

HESMesh::HESMesh(const HESMesh& other)
{
    *this = other;
}

HESMesh::~HESMesh()
{
    for(auto& e : m_Edges)
        delete e;
}

void HESMesh::clear()
{
    for(const auto& edge : m_Edges)
        delete edge;

    m_Edges.clear();

    Mesh::clear();
}

size_t HESMesh::getNumEdges() const
{
    return m_Edges.size();
}

HESEdge* HESMesh::getHESEdge(const size_t& idx) const
{
    assert(idx >= 0 && idx < m_Edges.size());

    return m_Edges[idx];
}

HESVertex* HESMesh::getHESVertex(const size_t& idx) const
{
    return dynamic_cast<HESVertex*>(getVertex(idx));
}

HESFace* HESMesh::getHESFace(const size_t& idx) const
{
    return dynamic_cast<HESFace*>(getFace(idx));
}

const std::vector<HESEdge*>& HESMesh::getEdges() const
{
    return m_Edges;
}

const bool& HESMesh::isChecked() const
{
    return m_Checked;
}

bool HESMesh::isManifold() const
{
    return m_BoundaryStartEdges.size() < 2;
}

const bool& HESMesh::isValid() const
{
    return m_Valid;
}

size_t HESMesh::getNumBoundaries() const
{
    return m_BoundaryStartEdges.size();
}

void HESMesh::addEdge(HESEdge* const edge)
{
    m_Edges.push_back(edge);
}

Vertex* HESMesh::allocateVertex()
{
    return new HESVertex();
}

Face* HESMesh::allocateFace()
{
    return new HESFace();
}

HESMesh& HESMesh::operator=(const HESMesh& other)
{
    clear();

    for(const auto& v : other.m_Vertices)
        addVertex(*v);

    for(const auto& f : other.m_Faces)
        addFace(*f);

    m_Valid = HESBuilder(this).build();
    m_Checked = false;

    return *this;
}

void HESMesh::addBoundaryStartEdge(HESEdge* const edge)
{
    m_BoundaryStartEdges.insert(edge);
}

void HESMesh::setChecked(const bool& checked)
{
    m_Checked = checked;
}

void HESMesh::setValid(const bool& valid)
{
    m_Valid = valid;
}

void HESMesh::setDirty()
{
    m_Checked = false;
    m_Valid = false;

    m_BoundaryStartEdges.clear();
}

std::set<HESEdge*> HESMesh::getBoundaryStartEdges() const
{
    return m_BoundaryStartEdges;
}

bool HESMesh::walkBoundary(HESEdge* const edge, HESEdgeConstVector& boundary)
{
    // if edge has a opposite, it is no boundary edge -> return nothing
    if(edge->opposite() != nullptr)
        return false;

    // set edge to process
    HESEdge* e = edge;

    // add edge to boundary
    boundary.push_back(e);

    // set start edge, need to be compared while looping through
    HESEdge* const e_start = e;
    bool loop_closed = false;
    bool found_next_edge = false;

    // while loop is not closed (not found e_start)
    while(!loop_closed)
    {
        found_next_edge = false;

        // iterate over all outgoing edges of the to vertex of the current edge
        for(const auto& e_out : e->to()->getEdges())
        {
            // if we found the starting edge -> loop is closed, nothing to process anymore
            if(e_out == e_start)
            {
                loop_closed = true;

                break;
            }

            // if current outgoing edge is no border edge (opposite is not null) -> continue
            if(e_out->opposite() != nullptr)
                continue;

            // we found the next border edge
            /// set current edge to e and remind us that we found the next border edge -> break
            e = e_out;
            found_next_edge = true;

            break;
        }

        // if lopp is closed -> nothing to process here -> break
        if(loop_closed)
            break;

        // if we did not found a next edge -> mesh is dirty -> break
        if(!found_next_edge)
            break;

        // add current e to boundary
        boundary.push_back(e);
    }

    // if did not we found a boundary -> return nothing
    if(!loop_closed)
        return false;

    // return boundary
    return true;
}

HESVertex* const HESMesh::splitEdgeAtPoint(HESEdge* const splitEdge, const Vec2d& splitPoint)
{
    HESVertex* new_vertex = new HESVertex();
    new_vertex->setPosition(splitPoint);
    
    size_t new_vertex_idx = m_Vertices.size();
    m_Vertices.push_back(new_vertex);

    // get opposite edge
    HESEdge* opp_split_edge = splitEdge->opposite();

    // insert vertex on splitedge
    HESEdge* const new_edge = insertVertexOnEdge(new_vertex, new_vertex_idx, splitEdge);

    // if we have no opposite -> nothing to do here -> return new vertex
    if(opp_split_edge == nullptr)
        return new_vertex;

    // insert vertex on opposite splitedge
    HESEdge* const new_opp_edge = insertVertexOnEdge(new_vertex, new_vertex_idx, opp_split_edge);

    // set opposites on edges
    splitEdge->setOpposite(new_opp_edge);
    new_opp_edge->setOpposite(splitEdge);

    opp_split_edge->setOpposite(new_edge);
    new_edge->setOpposite(opp_split_edge);

    // return new vertex
    return new_vertex;
}

HESEdge* const HESMesh::insertVertexOnEdge(HESVertex* vertex, const size_t& vertexId, HESEdge* const edge)
{
    HESEdge* new_edge = new HESEdge();

    // set vertices on edges
    new_edge->setFrom(vertex);
    new_edge->setTo(edge->to());

    edge->setTo(vertex);

    // set edges on edges
    new_edge->setNext(edge->next());
    edge->next()->setPrev(new_edge);

    edge->setNext(new_edge);
    new_edge->setPrev(edge);

    // set edges on faces
    edge->face()->addEdge(new_edge);

    // set faces on edges
    new_edge->setFace(edge->face());

    // set edges on vertices
    vertex->addEdge(new_edge);

    // set opposites to nullptr
    edge->setOpposite(nullptr);
    new_edge->setOpposite(nullptr);

    // add idx on face
    const std::vector<size_t>& idxs = edge->face()->getVertIds();
    std::vector<size_t> new_idxs;

    for(const auto& idx : idxs)
    {
        new_idxs.push_back(idx);

        if(getHESVertex(idx) == edge->from())
            new_idxs.push_back(vertexId);
    }

    edge->face()->setVertIdx(new_idxs);

    // return the new edge
    return new_edge;
}
