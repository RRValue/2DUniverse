#ifndef _RENDERING_QWIDGETTARGET_H_
#define _RENDERING_QWIDGETTARGET_H_

#include "HalfEdge2D/Rendering/RenderTarget.h"

#include <QtWidgets/QWidget>

class RenderWidget;

class QWidgetTarget : public QObject, public RenderTarget
{
    Q_OBJECT

public:
    QWidgetTarget() = delete;
    QWidgetTarget(RenderWidget* renderWidget);
    ~QWidgetTarget();

    QPaintDevice* const getPaintDevice() const;
    
    virtual void render() final override;

private slots:
    void handleMouseMoveEvent(QMouseEvent* event);
    void handleMousePressEvent(QMouseEvent* event);
    void handleMouseReleaseEvent(QMouseEvent* event);
    void handleResizeEvent(QResizeEvent* event);
    void handlePaintEvent(QPaintEvent* event);
    void handleWheelEvent(QWheelEvent* event);

private:
    RenderWidget* const m_RenderWidget;
};

#endif //_RENDERING_QWIDGETTARGET_H_
