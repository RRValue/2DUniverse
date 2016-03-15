#ifndef _HALFEDGE_APPLICATION_H_
#define _HALFEDGE_APPLICATION_H_

#include <QtWidgets/QApplication>

class QPaintTarget;

class EventHandler;
class EventInterface;
class Renderer;

class Navigator;
class Controller;

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
    void initTest();
};

#endif //_HALFEDGE_APPLICATION_H_
