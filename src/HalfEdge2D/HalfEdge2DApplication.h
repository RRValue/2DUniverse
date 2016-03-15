#ifndef _HALFEDGE_APPLICATION_H_
#define _HALFEDGE_APPLICATION_H_

#include <QtWidgets/QApplication>

#include "HalfEdge2D/MainWindow_uic.h"

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

private:
    Ui_HalfEdgeAppMainWindow m_MainWindowForm;
};

#endif //_HALFEDGE_APPLICATION_H_
