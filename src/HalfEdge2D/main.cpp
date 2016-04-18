#include "HalfEdge2D/HalfEdge2DApplication.h"

#include <QtCore/QTimer>

#include "HalfEdge2D/Base/StaticNGradeBezier.h"

#define T float
#define G 2
#define D 2
#define N (G + 1)

typedef StaticNGradeBezier<T, G, D> QBez2F;

int main(int argc, char** argv)
{
    QBez2F qbezier2f;
    qbezier2f.setPoint(0, Vec2f(0.0f, 0.0f));
    qbezier2f.setPoint(1, Vec2f(3.0f, 3.0f));
    qbezier2f.setPoint(2, Vec2f(2.0f, 0.0f));

    Vec2f p = qbezier2f.pointAt(0.5);
    Vec2f t = qbezier2f.tangentAt(0.5);
    Vec2f n = qbezier2f.normalAt(0.5);
    T k = qbezier2f.curvationAt(0.5);

    HalfEdge2DApplication app(argc, argv);

    QTimer::singleShot(0, &app, &HalfEdge2DApplication::onRun);

    return app.exec();
}
