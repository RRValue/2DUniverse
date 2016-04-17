#ifndef _RENDERABLE_QUDRATICBEZIER_H_
#define _RENDERABLE_QUDRATICBEZIER_H_

#include "HalfEdge2D/Base/StaticNGradeBezier.h"

#include "HalfEdge2D/Renderables/Renderable.h"

typedef StaticNGradeBezier<float, 2, 2> QBez2F;

class QuadraticBezier : public QBez2F, public Renderable
{
public:
    QuadraticBezier();
    QuadraticBezier(const QuadraticBezier& other);
    ~QuadraticBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getThickness() const;

    //setter
    void setThickness(const float& thickness);;

private:
    float m_Thickness;
};


#endif //_RENDERABLE_QUDRATICBEZIER_H_
