#include "HalfEdge2D/HalfEdge2DApplication.h"

#include <QtCore/QTimer>

#include "HalfEdge2D/Renderables/Spline.h"

int main(int argc, char** argv)
{
    Spline spline;

    spline.addPoint(Vec2f(0.0f, 0.0f));
    spline.addPoint(Vec2f(1.0f, 1.0f));
    spline.addPoint(Vec2f(2.0f, 0.0f));

    HalfEdge2DApplication app(argc, argv);

    QTimer::singleShot(0, &app, &HalfEdge2DApplication::onRun);

    return app.exec();
}
