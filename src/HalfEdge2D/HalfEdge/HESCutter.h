#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include <vector>

class HESMesh;
class Line;
class Point;

typedef std::vector<Point*> CutPointVector;
typedef std::vector<HESMesh*> HESMeshVector;

class HESCutter
{
public:
    HESCutter();
    ~HESCutter();

    const CutPointVector& getCutPoints() const;

    bool cutLine(HESMesh* const sourceMesh, Line* const line, HESMeshVector& outMeshes);

private:
    void clearCutPoints();

private:
    HESMesh* m_SourceMesh;
    HESMesh* m_TargetMesh;

    CutPointVector m_CutPoints;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
