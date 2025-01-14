#ifndef _RENDERING_RENDERTARGET_H_
#define _RENDERING_RENDERTARGET_H_

#include "2DUniverse/Base/Vector.h"

#include <QtCore/QSizeF>

#include <vector>

class ViewPort;
class EventInterface;

class RenderTarget
{
public:
    RenderTarget();
    ~RenderTarget();

    virtual void render() = 0;

    // getter
    const QSizeF& getSize() const;
    const std::vector<ViewPort*>& getViewPorts() const;
    ViewPort* const getViewPort(const size_t& idx) const;
    const Mat3f& getDeviceMatrix();
    const Mat3f& getInvDeviceMatrix();

    // setter
    void setEventHandler(EventInterface* const eventInterface);
    void setSize(const QSizeF& size);
    void addViewPort(ViewPort* const viewport);
    void removeViewPort(ViewPort* const viewport);
    void clearViewPorts();

    // tooling
    void updateViewPortsTargetSize();

protected:
    EventInterface* m_EventInterface;

    QSizeF m_Size;

    std::vector<ViewPort*> m_ViewPorts;

private:
    void updateDeviceMatrices();

private:
    bool m_DeviceDirty;

    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
};

#endif //_RENDERING_RENDERTARGET_H_
