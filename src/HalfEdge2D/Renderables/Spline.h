#ifndef _RENDERABLE_SPLINE_H_
#define _RENDERABLE_SPLINE_H_

#include "HalfEdge2D/Base/StaticSpline.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"

class Spline : public Spline2F, public RenderableLine
{
public:
    Spline();
    Spline(const Spline& other);
    Spline(Spline&& other);
    ~Spline();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
};

#endif //_RENDERABLE_SPLINE_H_
