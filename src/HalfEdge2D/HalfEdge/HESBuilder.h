#ifndef _HALFEDGESTRUCTURE_BUILDER_H_
#define _HALFEDGESTRUCTURE_BUILDER_H_

#include <vector>

class HESMesh;
class HESEdge;
class HESVertex;
class HESFace;

class HESBuilder
{
public:
    HESBuilder();
    HESBuilder(HESMesh* const mesh);
    ~HESBuilder();

    bool build();
    bool build(HESMesh* const mesh);

private:
    void buildFace(const std::vector<HESVertex*>& vertices, const std::vector<HESEdge*>& edges, HESFace* const face);
    void connectEdges(const std::vector<HESEdge*>& edges);

private:
    HESMesh* m_Mesh;
};

#endif //_HALFEDGESTRUCTURE_BUILDER_H_
