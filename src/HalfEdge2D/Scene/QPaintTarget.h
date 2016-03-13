#ifndef _SCENE_QPAINTTARGET_H_
#define _SCENE_QPAINTTARGET_H_

#include "HalfEdge2D/Scene/RenderTarget.h"

#include <QtWidgets/QWidget>

class QPaintTarget : public QWidget, public RenderTarget
{
public:
    QPaintTarget(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    ~QPaintTarget();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) final override;
    virtual void mousePressEvent(QMouseEvent* event) final override;
    virtual void mouseReleaseEvent(QMouseEvent* event) final override;
    virtual void resizeEvent(QResizeEvent* event) final override;
    virtual void paintEvent(QPaintEvent* event) final override;
    virtual void wheelEvent(QWheelEvent* event) final override;

    virtual void render() final override;
};

#endif //_SCENE_QPAINTTARGET_H_
