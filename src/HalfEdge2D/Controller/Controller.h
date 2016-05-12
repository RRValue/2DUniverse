#ifndef _CONTROLLER_CONTROLLER_H_
#define _CONTROLLER_CONTROLLER_H_

#include "HalfEdge2D/Events/EventInterface.h"

#include "HalfEdge2D/Base/Vector.h"

class Scene;
class Camera;
class EventHandler;

class QWidget;

class Controller : public EventInterface
{
    friend class EventHandler;

public:
    Controller();
    virtual ~Controller();

    virtual void init() = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;

    const bool& isActive() const;
    const std::string& getName() const;

    QWidget* const getOptionWidget() const;
    Scene* const getScene() const;

    void setScene(Scene* const scene);
    void setChanged();

protected:
    bool inViewPort(const Vec2i& point) const;
    Vec2i keepInViewPort(const Vec2i& point) const;

    void updateTransMatrix();
    Vec2d trans(const Vec2d& point);
    Vec2d invTrans(const Vec2d& point);

protected:
    virtual void preSceneChanged();
    virtual void postSceneChanged();

protected:
    bool m_Active;
    std::string m_Name;
    bool m_ViewportContentChanges;
    bool m_SceneChanges;

    Scene* m_Scene;

    // scene matrices
    Mat3f m_DeviceMat;
    Mat3f m_InvDeviceMat;
    Mat3f m_TransMat;
    Mat3f m_InvTransMat;

    // option widget
    QWidget* m_OptionWidget;
};

#endif //_CONTROLLER_CONTROLLER_H_
