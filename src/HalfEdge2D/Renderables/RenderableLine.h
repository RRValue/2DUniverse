#ifndef _RENDERABLE_RENDERABLELINE_H_
#define _RENDERABLE_RENDERABLELINE_H_

#include "HalfEdge2D/Base/Vector.h"

#include "HalfEdge2D/Renderables/Renderable.h"

class RenderableLine : public Renderable
{
public:
    RenderableLine();
    RenderableLine(const RenderableLine& other);
    ~RenderableLine();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    // getter
    const float& getThickness() const;

    //setter
    void setThickness(const float& thickness);

private:
    float m_Thickness;
};

#endif //_RENDERABLE_RENDERABLELINE_H_
