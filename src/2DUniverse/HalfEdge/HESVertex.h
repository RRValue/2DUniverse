#ifndef _HALFEDGESTRUCTURE_VERTEX_H_
#define _HALFEDGESTRUCTURE_VERTEX_H_

#include "2DUniverse/Mesh/Vertex.h"

#include <set>

class HESEdge;

class HESVertex : public Vertex
{
public:
    HESVertex();
    HESVertex(const HESVertex& other);
    virtual ~HESVertex();

    // getter
    const std::set<HESEdge*>& getEdges() const;
    const bool& visited() const;

    // setter
    void setVisited(const bool& visited);

    // edges
    void addEdge(HESEdge* const edge);
    void removeEdge(HESEdge* const edge);

private:
    std::set<HESEdge*>::const_iterator findEdge(HESEdge* const edge);

private:
    std::set<HESEdge*> m_Edges;

    bool m_Visited;
};

#endif //_HALFEDGESTRUCTURE_VERTEX_H_
