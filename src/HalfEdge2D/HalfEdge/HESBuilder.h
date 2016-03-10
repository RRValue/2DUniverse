#ifndef _HALFEDGESTRUCTURE_BUILDER_H_
#define _HALFEDGESTRUCTURE_BUILDER_H_

#include <array>
#include <initializer_list>

class HESMesh;
class HESEdge;
class HESVertex;
class HESFace;

class HESBuilder
{
public:
    HESBuilder() = delete;
    HESBuilder(HESMesh* const mesh);
    ~HESBuilder();

    bool build();

private:
    void buildFace(const std::array<HESVertex* const, 3>& vertices, const std::array<HESEdge* const, 3>& edges, HESFace* const face);
    void connectEdges(const std::initializer_list<HESEdge* const>& edges);

private:
    HESMesh* const m_Mesh;
};

#endif //_HALFEDGESTRUCTURE_BUILDER_H_
