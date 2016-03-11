#ifndef _HALFEDGE_APPLICATION_H_
#define _HALFEDGE_APPLICATION_H_

#include <QtWidgets/QApplication>

class HalfEdge2DWidget;
class HESMesh;

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

    HESMesh* m_Mesh;
};

#endif //_HALFEDGE_APPLICATION_H_
