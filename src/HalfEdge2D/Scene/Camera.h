#ifndef _SCENE_CAMERA_H_
#define _SCENE_CAMERA_H_

#include <QtCore/QPointF>

class Canvas;

class Camera
{
public:
    Camera() = delete;
    Camera(Canvas* const canvas);
    ~Camera();

    // getter
    const QPointF& getOrthoSize() const;
    const QPointF& getPosition() const;
    Canvas* const getCanvas() const;

    // setter
    void setOrthoSize(const QPointF& orthoSize);
    void setPosition(const QPointF& position);

private:
    Canvas* const m_Canvas;
    QPointF m_OrthoSize;
    QPointF m_Position;
};

#endif //_SCENE_CAMERA_H_
