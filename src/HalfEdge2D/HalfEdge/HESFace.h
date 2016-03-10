#ifndef _HALFEDGESTRUCTURE_FACE_H_
#define _HALFEDGESTRUCTURE_FACE_H_

#include <set>

class HESEdge;

class HESFace
{
public:
    HESFace();
    ~HESFace();

    // getter
    const std::set<HESEdge*>& getEdges() const;

    // edges
    void addEdge(HESEdge* const edge);
    void removeEdge(HESEdge* const edge);

private:
    std::set<HESEdge*>::const_iterator findEdge(HESEdge* const edge);

private:
    std::set<HESEdge*> m_Edges;
};

#endif //_HALFEDGESTRUCTURE_FACE_H_
