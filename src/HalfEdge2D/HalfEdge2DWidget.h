#ifndef _HALFEDGE_WIDGET_H_
#define _HALFEDGE_WIDGET_H_

#include <QtWidgets/QWidget>

#include <QtCore/QVector>

class HalfEdge2DEventInterface;

class HalfEdge2DWidget : public QWidget
{
    Q_OBJECT

public:
    HalfEdge2DWidget(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    ~HalfEdge2DWidget();

    void setEventHandler(HalfEdge2DEventInterface* const eventInterface);

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) final override;
    virtual void mousePressEvent(QMouseEvent* event) final override;
    virtual void mouseReleaseEvent(QMouseEvent* event) final override;
    virtual void resizeEvent(QResizeEvent* event) final override;
    virtual void paintEvent(QPaintEvent* event) final override;
    virtual void wheelEvent(QWheelEvent* event) final override;

private:
    HalfEdge2DEventInterface* m_EventInterface;
};

#endif //_HALFEDGE_WIDGET_H_
