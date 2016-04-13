#include "HalfEdge2D/HalfEdge2DApplication.h"

#include <QtCore/QTimer>

#include "HalfEdge2D/Base/Vector.h"
#include "HalfEdge2D/Base/StaticPolynomialSolver.h"

int main(int argc, char** argv)
{
    StaticPolynomialSolver<float, 1> bla;
    StaticPolynomialSolver<float, 1>::Result res = bla.solve({1, 2});

    float a = 1.0f;
    float b = 2.0f;

    std::array<float, 2> aa = {1, 2};

    res = bla.solve(aa);
    res = bla.solve({&a, &b});

    HalfEdge2DApplication app(argc, argv);

    QTimer::singleShot(0, &app, &HalfEdge2DApplication::onRun);

    return app.exec();
}
