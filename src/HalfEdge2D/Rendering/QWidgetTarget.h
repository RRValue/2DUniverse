#ifndef _RENDERING_QWIDGETTARGET_H_
#define _RENDERING_QWIDGETTARGET_H_

#include "HalfEdge2D/Rendering/RenderTarget.h"

#include <QtWidgets/QWidget>

class QWidgetTarget : public QWidget, public RenderTarget
{
public:
    QWidgetTarget(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    ~QWidgetTarget();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) final override;
    virtual void mousePressEvent(QMouseEvent* event) final override;
    virtual void mouseReleaseEvent(QMouseEvent* event) final override;
    virtual void resizeEvent(QResizeEvent* event) final override;
    virtual void paintEvent(QPaintEvent* event) final override;
    virtual void wheelEvent(QWheelEvent* event) final override;

    virtual void render() final override;
};

#endif //_RENDERING_QWIDGETTARGET_H_
