#ifndef _HALFEDGE_WIDGET_H_
#define _HALFEDGE_WIDGET_H_

#include <QtWidgets/QWidget>

#include <QtCore/QVector>

class HalfEdge2DWidget : public QWidget
{
    Q_OBJECT

public:
    HalfEdge2DWidget(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    ~HalfEdge2DWidget();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) final override;
    virtual void mousePressEvent(QMouseEvent* event) final override;
    virtual void mouseReleaseEvent(QMouseEvent* event) final override;
    virtual void resizeEvent(QResizeEvent* event) final override;
    virtual void paintEvent(QPaintEvent* event) final override;
    virtual void wheelEvent(QWheelEvent* event) final override;

private:
    bool inWidget(const QPoint& point);
    QPoint keepInWidget(const QPoint& point);
    
    void updateRelativeBases();

    QPointF toView(const QPointF& p);
    QPointF fromView(const QPointF& p);

    QPointF toDeviceCoords(const QPointF& point);
    QPointF fromDeviceCoords(const QPointF& point);

    QPointF toWidgetCoords(const QPointF& point);
    QPointF fromWidgetCoords(const QPointF& point);

    QPointF transform(const QPointF& point);
    QPointF invTransform(const QPointF& point);

    int getHitPoint(const QPointF& pos);

    void zoom(const int& step, const QPoint& pos_px);

private:
    // widget
    QSize _current_size;
    float _aspect_ratio;
    
    // camera
    QPointF _ortho_size;
    QPointF _camera_position;

    // points
    QVector<QPointF> _points;
    float _point_size_px;

    // point movment
    int _current_idx;
    QPointF _current_hit_distance;

    // flag manip process
    bool _move_point_mode;
    bool _move_cam_mode;

    // scene manipulation
    float _zoom_factor;
    QPointF _cam_move_init_mouse_pos;
    QPointF _cam_move_init_cam_pos;
};

#endif //_HALFEDGE_WIDGET_H_
