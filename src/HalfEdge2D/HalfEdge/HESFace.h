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
    const std::set<HESEdge*>& getBoundingEdges() const;

    // edges
    void addBoundingEdge(HESEdge* const edge);
    void removeBoundingEdge(HESEdge* const edge);

private:
    std::set<HESEdge*>::const_iterator findBoundingEdge(HESEdge* const edge);

private:
    std::set<HESEdge*> m_BoundingEdges;
};

#endif //_HALFEDGESTRUCTURE_FACE_H_
