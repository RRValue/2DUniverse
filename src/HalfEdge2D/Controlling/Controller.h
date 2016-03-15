#ifndef _CONTROLLING_CONTROLLER_H_
#define _CONTROLLING_CONTROLLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QPointF>

class Scene;
class Camera;

class Controller : public EventInterface
{
public:
    Controller();
    virtual ~Controller();

    void setScene(Scene* const scene);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    bool inViewPort(const QPoint& point) const;
    QPoint keepInViewPort(const QPoint& point) const;

    void updateTransMatrix();
    QPointF trans(const QPointF& point);
    QPointF invTrans(const QPointF& point);

private:
    bool m_MovePoint;

    Scene* m_Scene;
    Camera* m_ActiveCamera;

    int m_CurrentIdx;
    QPointF m_CurrentHitDistance;

    // scene matrices    
    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;
};

#endif //_CONTROLLING_CONTROLLER_H_
