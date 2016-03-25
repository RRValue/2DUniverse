#ifndef _HALFEDGESTRUCTURE_FACE_H_
#define _HALFEDGESTRUCTURE_FACE_H_

#include "HalfEdge2D/Mesh/Face.h"

#include <set>

class HESEdge;

class HESFace : public Face
{
public:
    HESFace();
    virtual ~HESFace();

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

#endif //_HALFEDGESTRUCTURE_FACE_H_
