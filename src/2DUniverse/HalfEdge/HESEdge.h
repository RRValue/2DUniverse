#ifndef _HALFEDGESTRUCTURE_EDGE_H_
#define _HALFEDGESTRUCTURE_EDGE_H_

class HESVertex;
class HESFace;

class HESEdge
{
public:
    HESEdge();
    ~HESEdge();

    // getter
    HESVertex* const from() const;
    HESVertex* const to() const;
    HESFace* const face() const;
    HESEdge* const opposite() const;
    HESEdge* const prev() const;
    HESEdge* const next() const;
    const bool& visited() const;

    // setter
    void setFrom(HESVertex* const vertex);
    void setTo(HESVertex* const vertex);
    void setFace(HESFace* const face);
    void setOpposite(HESEdge* const oppositeEdge);
    void setPrev(HESEdge* const prevEdge);
    void setNext(HESEdge* const nextEdge);
    void setVisited(const bool& visited);

private:
    HESVertex* m_From;
    HESVertex* m_To;
    
    HESFace* m_Face;

    HESEdge* m_Opposite;
    HESEdge* m_Prev;
    HESEdge* m_Next;

    bool m_Visited;
};

#endif //_HALFEDGESTRUCTURE_EDGE_H_
