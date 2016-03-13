#ifndef _SCENE_VIEWPORT_H_
#define _SCENE_VIEWPORT_H_

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QRectF>
#include <QtCore/QSizeF>

class Camera;

class ViewPort
{
public:
    ViewPort();
    ~ViewPort();

    // getter
    const QRectF& getSize() const;
    const QSizeF& getTargetSize() const;
    Camera* const getCamera() const;
    const Mat3f& getProjectionMatrix();
    const Mat3f& getInvProjectionMatrix();

    // setter
    void setSize(const QRectF& size);
    void setTargetSize(const QSizeF& targetSize);
    void setCamera(Camera* const camera);

private:
    void updateProjectionMatrix();

private:
    Camera* m_Camera;
    
    QRectF m_Size;
    QSizeF m_TargetSize;

    Mat3f m_ProjectionMatrix;
    Mat3f m_InvProjectionMatrix;

    bool m_ProjectionDirty;
};

#endif //_SCENE_VIEWPORT_H_
