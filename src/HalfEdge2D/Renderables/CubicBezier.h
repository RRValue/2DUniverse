#ifndef _RENDERABLE_CUBICBEZIER_H_
#define _RENDERABLE_CUBICBEZIER_H_

#include "HalfEdge2D/Base/StaticNGradeBezier.h"

#include "HalfEdge2D/Renderables/Renderable.h"

typedef StaticNGradeBezier<float, 3, 2> CBez2F;

class CubicBezier : public CBez2F, public Renderable
{
public:
    CubicBezier();
    CubicBezier(const CubicBezier& other);
    ~CubicBezier();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const float& getThickness() const;
    
    //setter
    void setThickness(const float& thickness);

private:
    float m_Thickness;
};

#endif //_RENDERABLE_CUBICBEZIER_H_
