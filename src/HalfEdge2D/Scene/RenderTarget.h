#ifndef _SCENE_RENDERTARGET_H_
#define _SCENE_RENDERTARGET_H_

#include <QtCore/QSizeF>

#include <vector>

class ViewPort;
class HalfEdge2DEventInterface;

class RenderTarget
{
public:
    RenderTarget();
    ~RenderTarget();

    virtual void render() = 0;

    // getter
    const QSizeF& getSize() const;
    const std::vector<ViewPort* const>& getViewPorts() const;
    ViewPort* const getViewPort(const size_t& idx) const;

    // setter
    void setEventHandler(HalfEdge2DEventInterface* const eventInterface);
    void setSize(const QSizeF& size);
    void addViewPort(ViewPort* const viewport);
    void removeViewPort(ViewPort* const viewport);

    // tooling
    void updateViewPortsTargetSize();

protected:
    HalfEdge2DEventInterface* m_EventInterface;

private:
    QSizeF m_Size;
    std::vector<ViewPort* const> m_ViewPorts;
};

#endif //_SCENE_RENDERTARGET_H_
