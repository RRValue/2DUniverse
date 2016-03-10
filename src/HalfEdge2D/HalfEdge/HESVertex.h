#ifndef _HALFEDGESTRUCTURE_VERTEX_H_
#define _HALFEDGESTRUCTURE_VERTEX_H_

#include "HalfEdge2D/Mesh/Vertex.h"

#include <set>

class HESEdge;

class HESVertex : public Vertex
{
public:
    HESVertex();
    virtual ~HESVertex();

    // getter
    const std::set<HESEdge*>& getIncidentEdges() const;

    // edges
    void addIncidentEdge(HESEdge* const edge);
    void removeIncidentEdge(HESEdge* const edge);

private:
    std::set<HESEdge*>::const_iterator findIncidentEdge(HESEdge* const edge);

private:
    std::set<HESEdge*> m_IncidentEdges;
};

#endif //_HALFEDGESTRUCTURE_VERTEX_H_
