#ifndef _RENDERING_RENDERWIDGET_H_
#define _RENDERING_RENDERWIDGET_H_

#include <QtWidgets/QWidget>

class RenderWidget : public QWidget
{
    Q_OBJECT

public:
    RenderWidget(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    ~RenderWidget();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) final override;
    virtual void mousePressEvent(QMouseEvent* event) final override;
    virtual void mouseReleaseEvent(QMouseEvent* event) final override;
    virtual void resizeEvent(QResizeEvent* event) final override;
    virtual void paintEvent(QPaintEvent* event) final override;
    virtual void wheelEvent(QWheelEvent* event) final override;

signals:
    void onMouseMoveEvent(QMouseEvent* event);
    void onMousePressEvent(QMouseEvent* event);
    void onMouseReleaseEvent(QMouseEvent* event);
    void onResizeEvent(QResizeEvent* event);
    void onPaintEvent(QPaintEvent* event);
    void onWheelEvent(QWheelEvent* event);
};

#endif //_RENDERING_RENDERWIDGET_H_
