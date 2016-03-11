#ifndef _HALFEDGE_APPLICATION_H_
#define _HALFEDGE_APPLICATION_H_

#include <QtWidgets/QApplication>

class HalfEdge2DWidget;
class HalfEdge2DEventHandler;
class HalfEdge2DEventInterface;

class HalfEdge2DNavigator;
class HalfEdge2DController;

class Scene;
class Camera;
class Canvas;

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
    HalfEdge2DWidget* m_MainWidget;
    HalfEdge2DEventHandler* m_EventHandler;

    HalfEdge2DNavigator* m_Navigator;
    HalfEdge2DController* m_Controller;

    Scene* m_Scene;
    Camera* m_Camera;
    Canvas* m_Canvas;
};

#endif //_HALFEDGE_APPLICATION_H_
