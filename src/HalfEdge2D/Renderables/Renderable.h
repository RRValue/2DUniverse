#ifndef _RENDERABLE_RENDERABLE_H_
#define _RENDERABLE_RENDERABLE_H_

#include "HalfEdge2D/Base/Vector.h"

class Renderable
{
public:
    Renderable();
    Renderable(const Renderable& other);
    ~Renderable();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    const Vec4f& getColour() const;
    const bool& isVisible() const;

    //setter
    void setColour(const Vec4f& colour);
    void setVisible(const bool& visible);

private:
    Vec4f m_Colour;
    bool m_Visible;
};

#endif //_RENDERABLE_RENDERABLE_H_
