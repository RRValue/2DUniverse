#ifndef _HALFEDGESTRUCTURE_BUILDER_H_
#define _HALFEDGESTRUCTURE_BUILDER_H_

class HESMesh;

class HESBuilder
{
public:
    HESBuilder() = delete;
    HESBuilder(HESMesh* const mesh);
    ~HESBuilder();

    bool build();

private:
    HESMesh* const m_Mesh;
};

#endif //_HALFEDGESTRUCTURE_BUILDER_H_
