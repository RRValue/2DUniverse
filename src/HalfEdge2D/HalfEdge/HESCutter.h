#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include <array>
#include <initializer_list>

class HESMesh;
class Line;

class HESCutter
{
public:
    HESCutter() = delete;
    HESCutter(HESMesh* const mesh);
    ~HESCutter();

    bool cutLine(HESMesh* const outMesh, Line* const line);

private:
    HESMesh* const m_SourceMesh;
    HESMesh* m_TargetMesh;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
