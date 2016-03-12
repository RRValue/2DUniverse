#ifndef _SCENE_CAMERA_H_
#define _SCENE_CAMERA_H_

#include <QtCore/QPointF>
#include <QtCore/QRectF>

class Canvas;

class Camera
{
public:
    Camera() = delete;
    Camera(Canvas* const canvas);
    ~Camera();

    // getter
    const float& getZoom() const;
    const QPointF& getPosition() const;
    const QRectF& getViewport() const;
    Canvas* const getCanvas() const;

    // setter
    void setZoom(const float& zoom);
    void setPosition(const QPointF& position);
    void setViewport(const QRectF& viewport);

private:
    Canvas* const m_Canvas;
    
    float m_Zoom;
    QPointF m_Position;
    QRectF m_Viewport;
};

#endif //_SCENE_CAMERA_H_
