#include "HalfEdge2D/HalfEdge2DWidget.h"

#include <QtWidgets/QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    HalfEdge2DWidget* bw = new HalfEdge2DWidget();
    bw->show();

    return app.exec();
}
