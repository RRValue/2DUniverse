#ifndef _HALFEDGESTRUCTURE_CUTTER_H_
#define _HALFEDGESTRUCTURE_CUTTER_H_

#include <vector>

class HESMesh;
class Line;
class Point;

typedef std::vector<Point*> CutPointVector;

class HESCutter
{
public:
    HESCutter();
    HESCutter(HESMesh* const mesh);
    ~HESCutter();

    const CutPointVector& getCutPoints() const;

    bool cutLine(HESMesh* const outMesh, Line* const line);

private:
    void prepareTargetMesh();
    void clearCutPoints();

private:
    HESMesh* m_SourceMesh;
    HESMesh* m_TargetMesh;

    CutPointVector m_CutPoints;
};

#endif //_HALFEDGESTRUCTURE_CUTTER_H_
