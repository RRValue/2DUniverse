#ifndef _CONTROLLING_CONTROLLER_H_
#define _CONTROLLING_CONTROLLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include "HalfEdge2D/Base/Vector.h"

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
    bool inViewPort(const Vec2i& point) const;
    Vec2i keepInViewPort(const Vec2i& point) const;

    void updateTransMatrix();
    Vec2f trans(const Vec2f& point);
    Vec2f invTrans(const Vec2f& point);

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
