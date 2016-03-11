#include "HalfEdge2D/HalfEdge2DWidget.h"

#include "HalfEdge2D/HalfEdge2DEventInterface.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include <qdebug.h>

HalfEdge2DWidget::HalfEdge2DWidget(QWidget* parent, Qt::WindowFlags f) : 
    QWidget(parent, f), 
    m_EventInterface(nullptr)
{
    /*_ortho_size = QPointF(1.0f, 1.0f);
    _camera_position = QPointF(0.0, 0.0f);
    _aspect_ratio = 1.0f;
    _point_size_px = 5.0f;
    _zoom_factor = 1.05f;
    _move_point_mode = false;
    _move_cam_mode = false;*/
}

HalfEdge2DWidget::~HalfEdge2DWidget()
{

}

void HalfEdge2DWidget::setEventHandler(HalfEdge2DEventInterface* const eventInterface)
{
    m_EventInterface = eventInterface;
}

void HalfEdge2DWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMouseMoveEvent(event);

    /*if(!_move_cam_mode && !_move_point_mode)
        return;

    if(_move_point_mode)
    {
        QPointF pos = keepInWidget(event->pos()) + _current_hit_distance;
        _points[_current_idx] = invTransform(pos);
    }

    if(_move_cam_mode)
    {
        _camera_position = _cam_move_init_cam_pos;

        QPoint pos = keepInWidget(event->pos());
        QPointF current_delta = _cam_move_init_mouse_pos - invTransform(pos);

        _camera_position = _cam_move_init_cam_pos + current_delta;
    }

    repaint();

    event->accept();*/
}

void HalfEdge2DWidget::mousePressEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr || event == nullptr)
        return;

    m_EventInterface->handleMousePressEvent(event);

    /*if(_move_cam_mode || _move_point_mode)
        return;

    // set to move mode
    if(event->button() == Qt::LeftButton)
        _move_point_mode = true;
    if(event->button() == Qt::RightButton)
        _move_cam_mode = true;

    if(_move_cam_mode && _move_point_mode)
        return;

    if(!_move_cam_mode && !_move_point_mode)
        return;

    if(_move_point_mode)
    {
        QPointF p = keepInWidget(event->pos());

        int result = getHitPoint(p);

        // if hit nothing and point size < 4 -> add
        if(result == -1)
        {
            if(_points.size() < 4)
            {
                _points.push_back(invTransform(p));

                _move_point_mode = false;

                repaint();
            }

            _move_point_mode = false;
        }
        else
        {
            _current_idx = result;
            _current_hit_distance = transform(_points[_current_idx]) - p;
        }
    }

    if(_move_cam_mode)
    {
        QPoint pos_px = event->pos();

        if(!inWidget(pos_px))
        {
            _move_cam_mode = false;
            
            return;
        }

        _cam_move_init_mouse_pos = invTransform(pos_px);
        _cam_move_init_cam_pos = _camera_position;
    }

    event->accept();*/
}

void HalfEdge2DWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_EventInterface == nullptr)
        return;

    m_EventInterface->handleMouseReleaseEvent(event);

    /*if(event == nullptr)
        return;

    if(!_move_point_mode && !_move_cam_mode)
        return;

    if(_move_point_mode)
        _move_point_mode = false;

    if(_move_cam_mode)
        _move_cam_mode = false;

    event->accept();*/
}

void HalfEdge2DWidget::wheelEvent(QWheelEvent* event)
{
    if(m_EventInterface == nullptr)
        return;

    m_EventInterface->handleWheelEvent(event);

    /*if(event == nullptr)
        return;

    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if(!numPixels.isNull())
        zoom(numPixels.y(), event->pos());
    else if(!numDegrees.isNull())
    {
        QPoint numSteps = numDegrees / 15;
        
        zoom(numSteps.y(), event->pos());
    }

    event->accept();*/
}

void HalfEdge2DWidget::resizeEvent(QResizeEvent* event)
{
    if(m_EventInterface == nullptr)
        return;

    m_EventInterface->handleResizeEvent(event);

    /*if(event == nullptr)
        return;

    _current_size = event->size();

    updateRelativeBases();*/
}

void HalfEdge2DWidget::paintEvent(QPaintEvent* event)
{
    /*QPainter painter(this);

    for(const auto& p : _points)
        painter.drawEllipse(transform(p), _point_size_px, _point_size_px);

    painter.drawLine(transform(QPointF(-1.0f, 0.0f)), transform(QPointF(1.0f, 0.0f)));
    painter.drawLine(transform(QPointF(0.0f, -1.0f)), transform(QPointF(0.0f, 1.0f)));*/
}

/*int HalfEdge2DWidget::getHitPoint(const QPointF& pos)
{
    if(_points.empty())
        return -1;

    for(int i = 0; i < _points.size(); i++)
    {
        QPointF point_pos = transform(_points[i]);
        QPointF delta = point_pos - pos;
        float distance = std::sqrt(std::pow(delta.x(), 2) + std::pow(delta.y(), 2));

        if(distance < _point_size_px)
            return i;
    }

    return -1;
}*/
