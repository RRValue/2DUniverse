#ifndef _RENDERABLE_CUBICBEZIER_H_
#define _RENDERABLE_CUBICBEZIER_H_

#include "HalfEdge2D/Base/StaticNGradeBezier.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"

class CubicBezier : public CBezier2F, public RenderableLine
{
public:
    CubicBezier();
    CubicBezier(const CubicBezier& other);
    CubicBezier(CubicBezier&& other);
    ~CubicBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif //_RENDERABLE_CUBICBEZIER_H_
