#ifndef _RENDERING_QPAINTTARGET_H_
#define _RENDERING_QPAINTTARGET_H_

#include "HalfEdge2D/Rendering/RenderTarget.h"

class QPixmap;
class QPaintDevice;

class QPaintTarget : public RenderTarget
{
public:
    QPaintTarget() = delete;
    QPaintTarget(int width, int height);
    ~QPaintTarget();

    QPaintDevice* const getPaintDevice() const;
    virtual void render() final override;

    void resize(const int& width, const int& height);
    void save(const QString& fileName);

private:
    QPixmap* m_Image;
};

#endif //_RENDERING_QPAINTTARGET_H_
