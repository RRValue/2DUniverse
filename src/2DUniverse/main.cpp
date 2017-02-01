#include "2DUniverse/MainApplication.h"

#include <QtCore/QTimer>

int main(int argc, char** argv)
{
    MainApplication app(argc, argv);

    QTimer::singleShot(0, &app, &MainApplication::onRun);

    return app.exec();
}
