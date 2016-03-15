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

protected:
    bool inViewPort(const QPoint& point) const;
    QPoint keepInViewPort(const QPoint& point) const;

    void updateTransMatrix();
    QPointF trans(const QPointF& point);
    QPointF invTrans(const QPointF& point);

protected:
    // scene matrices    
    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;
};

#endif //_CONTROLLING_CONTROLLER_H_
