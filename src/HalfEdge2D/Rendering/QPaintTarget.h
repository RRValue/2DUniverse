#ifndef _RENDERING_QPAINTTARGET_H_
#define _RENDERING_QPAINTTARGET_H_

#include "HalfEdge2D/Rendering/RenderTarget.h"

class QImage;
class QPaintDevice;

class QPaintTarget : public RenderTarget
{
public:
    QPaintTarget() = delete;
    QPaintTarget(int width, int height);
    ~QPaintTarget();

    Vec4f getColourAtPos(const unsigned int& x, const unsigned int& y);

    QPaintDevice* const getPaintDevice() const;
    virtual void render() final override;

    void resize(const int& width, const int& height);
    void save(const QString& fileName);

private:
    QImage* m_Image;
};

#endif //_RENDERING_QPAINTTARGET_H_
