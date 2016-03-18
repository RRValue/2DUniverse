#ifndef _CONTROLLING_CONTROLLER_H_
#define _CONTROLLING_CONTROLLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include "HalfEdge2D/Base/Vector.h"

#include <QtCore/QPointF>

class Scene;
class Camera;
class EventHandler;

class Controller : public EventInterface
{
    friend class EventHandler;

public:
    Controller();
    virtual ~Controller();

    const bool& isActive() const;
    const std::string& getName() const;

protected:
    bool inViewPort(const QPoint& point) const;
    QPoint keepInViewPort(const QPoint& point) const;

    void updateTransMatrix();
    QPointF trans(const QPointF& point);
    QPointF invTrans(const QPointF& point);

protected:
    bool m_Active;
    std::string m_Name;
    bool m_ViewportContentChanges;

    // scene matrices    
    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;
};

#endif //_CONTROLLING_CONTROLLER_H_
