#ifndef _HALFEDGE_APPLICATION_H_
#define _HALFEDGE_APPLICATION_H_

#include <QtWidgets/QApplication>

class QPaintTarget;

class HalfEdge2DEventHandler;
class HalfEdge2DEventInterface;
class HalfEdge2DRenderer;

class HalfEdge2DNavigator;
class HalfEdge2DController;

class Scene;
class Camera;
class ViewPort;

class HalfEdge2DApplication : public QApplication
{
    Q_OBJECT

public:
    HalfEdge2DApplication() = delete;
    HalfEdge2DApplication(int& argc, char** argv);
    ~HalfEdge2DApplication();

public slots:
    void onRun();

private:
    void init();

private:
    QPaintTarget* m_PaintTarget;
    HalfEdge2DEventHandler* m_EventHandler;

    HalfEdge2DNavigator* m_Navigator;
    HalfEdge2DController* m_Controller;

    HalfEdge2DRenderer* m_Renderer;

    Scene* m_Scene;
    Camera* m_Camera;
    ViewPort* m_ViewPort0;
    ViewPort* m_ViewPort1;
    ViewPort* m_ViewPort2;
    ViewPort* m_ViewPort3;
};

#endif //_HALFEDGE_APPLICATION_H_
