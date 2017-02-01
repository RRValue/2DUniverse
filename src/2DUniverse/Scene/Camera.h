#ifndef _SCENE_CAMERA_H_
#define _SCENE_CAMERA_H_

#include "2DUniverse/Base/Vector.h"

#include <QtCore/QPointF>
#include <QtCore/QRectF>

class Camera
{
public:
    Camera();
    ~Camera();

    // getter
    const float& getZoom() const;
    const float& getRotation() const;
    const QPointF& getPosition() const;
    const QRectF& getViewport() const;
    const Mat3f& getViewMatrix();
    const Mat3f& getInvViewMatrix();

    // setter
    void setZoom(const float& zoom);
    void setRotation(const float& rotation);
    void setPosition(const QPointF& position);
    void setViewport(const QRectF& viewport);

private:
    void updateViewMatrix();

private:    
    float m_Zoom;
    float m_Rotation;
    QPointF m_Position;
    QRectF m_Viewport;

    bool m_ViewDirty;
    Mat3f m_ViewMatrix;
    Mat3f m_InvViewMatrix;

    const float m_2Pi;
};

#endif //_SCENE_CAMERA_H_
