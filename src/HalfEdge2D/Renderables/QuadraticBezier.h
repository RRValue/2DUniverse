#ifndef _RENDERABLE_QUDRATICBEZIER_H_
#define _RENDERABLE_QUDRATICBEZIER_H_

#include "HalfEdge2D/Base/StaticNGradeBezier.h"

#include "HalfEdge2D/Renderables/RenderableLine.h"

class QuadraticBezier : public QBezier2F, public RenderableLine
{
public:
    QuadraticBezier();
    QuadraticBezier(const QuadraticBezier& other);
    ~QuadraticBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
};


#endif //_RENDERABLE_QUDRATICBEZIER_H_
