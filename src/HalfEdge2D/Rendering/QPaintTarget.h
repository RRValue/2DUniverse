#ifndef _RENDERING_QPAINTTARGET_H_
#define _RENDERING_QPAINTTARGET_H_

#include "HalfEdge2D/Rendering/RenderTarget.h"

#include <QtGui/QPixmap>

class QPaintTarget : public QPixmap, public RenderTarget
{
public:
    QPaintTarget() = delete;
    QPaintTarget(int width, int height);
    ~QPaintTarget();

protected:
    virtual void render() final override;
};

#endif //_RENDERING_QPAINTTARGET_H_
