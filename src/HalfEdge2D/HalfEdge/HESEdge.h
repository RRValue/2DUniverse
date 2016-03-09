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
    HESVertex* const getSourceVertex() const;
    HESFace* const getIncidentFace() const;
    HESEdge* const getOppositeEdge() const;
    HESEdge* const getPrevEdge() const;
    HESEdge* const getNextEdge() const;

    // setter
    void setSourceVertex(HESVertex* const vertex);
    void setIncidentFace(HESFace* const face);
    void setOppositeEdge(HESEdge* const oppositeEdge);
    void setPrevEdge(HESEdge* const prevEdge);
    void setNextEdge(HESEdge* const nextEdge);

private:
    HESVertex* m_SourceVertex;
    
    HESFace* m_IncidentFace;

    HESEdge* m_OppositeEdge;
    HESEdge* m_PrevEdge;
    HESEdge* m_NextEdge;
};

#endif //_HALFEDGESTRUCTURE_EDGE_H_
