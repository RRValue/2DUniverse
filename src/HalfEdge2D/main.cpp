#include "HalfEdge2D/HalfEdge2DApplication.h"

#include <QtCore/QTimer

int main(int argc, char** argv)
{
    HalfEdge2DApplication app(argc, argv);

    QTimer::singleShot(0, &app, &HalfEdge2DApplication::onRun);

    return app.exec();
}
