#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/HalfEdge2DEventhandler.h"
#include "HalfEdge2D/HalfEdge2DController.h"
#include "HalfEdge2D/HalfEdge2DNavigator.h"
#include "HalfEdge2D/HalfEdge2DRenderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/QPaintTarget.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{

}

HalfEdge2DApplication::~HalfEdge2DApplication()
{

}

void HalfEdge2DApplication::onRun()
{
    init();
}

void HalfEdge2DApplication::init()
{
    // create a scene, camera and canvas
    Scene* scene = new Scene();
    
    Camera* camera = new Camera();

    ViewPort* viewPort = new ViewPort();
    viewPort->setSize(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
    viewPort->setCamera(camera);

    // allocate widget
    QPaintTarget* paintTarget = new QPaintTarget();
    
    // allocate event handler and add controller and navigator
    HalfEdge2DNavigator* navigator = new HalfEdge2DNavigator(paintTarget);
    HalfEdge2DController* controller = new HalfEdge2DController(paintTarget);

    controller->setScene(scene);

    // create renderer
    HalfEdge2DRenderer* renderer = new HalfEdge2DRenderer();
    renderer->setScene(scene);
    renderer->setWidget(paintTarget);

    // create event handler
    HalfEdge2DEventHandler* eventHandler = new HalfEdge2DEventHandler(paintTarget);
    eventHandler->addEventInterface(navigator);
    eventHandler->addEventInterface(controller);
    eventHandler->setRenderer(renderer);
    
    paintTarget->setEventHandler(eventHandler);
    paintTarget->addViewPort(viewPort);
    paintTarget->show();
}

void HalfEdge2DApplication::initTest()
{
    // init gui
    QWidget* main_widget = new QWidget();

    QHBoxLayout* horizontalLayout = new QHBoxLayout(main_widget);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    
    QPaintTarget* widget_multi = new QPaintTarget(main_widget);
    widget_multi->setObjectName(QStringLiteral("widget_multi"));
    widget_multi->setMinimumSize(QSize(2, 0));

    horizontalLayout->addWidget(widget_multi);

    QPaintTarget* widget_single = new QPaintTarget(main_widget);
    widget_single->setObjectName(QStringLiteral("widget_single"));
    widget_single->setMinimumSize(QSize(100, 0));
    widget_single->setMaximumSize(QSize(200, 16777215));

    horizontalLayout->addWidget(widget_single);

    QWidget* widget_3 = new QWidget(main_widget);
    widget_3->setObjectName(QStringLiteral("widget_3"));
    widget_3->setMinimumSize(QSize(100, 0));
    widget_3->setMaximumSize(QSize(100, 16777215));

    QVBoxLayout* verticalLayout = new QVBoxLayout(widget_3);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

    QPushButton* pushButton = new QPushButton(widget_3);
    pushButton->setObjectName(QStringLiteral("pushButton"));

    verticalLayout->addWidget(pushButton);

    QPushButton* pushButton_2 = new QPushButton(widget_3);
    pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

    verticalLayout->addWidget(pushButton_2);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);

    horizontalLayout->addWidget(widget_3);

    // init scene stuff
    // create a scene, camera and canvas
    Scene* scene = new Scene();

    Camera* camera0 = new Camera();
    Camera* camera1 = new Camera();
    Camera* camera2 = new Camera();
    Camera* camera3 = new Camera();

    ViewPort* viewPort0 = new ViewPort();
    viewPort0->setSize(QRectF(0.0f, 0.0f, 0.5f, 0.5f));
    viewPort0->setCamera(camera0);

    ViewPort* viewPort1 = new ViewPort();
    viewPort1->setSize(QRectF(0.5f, 0.0f, 0.5f, 0.5f));
    viewPort1->setCamera(camera1);

    ViewPort* viewPort2 = new ViewPort();
    viewPort2->setSize(QRectF(0.0f, 0.5f, 0.5f, 0.5f));
    viewPort2->setCamera(camera2);

    ViewPort* viewPort3 = new ViewPort();
    viewPort3->setSize(QRectF(0.5f, 0.5f, 0.5f, 0.5f));
    viewPort3->setCamera(camera3);

    ViewPort* viewPort4 = new ViewPort();
    viewPort4->setSize(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
    viewPort4->setCamera(camera0);

    // allocate event handler and add controller and navigator
    HalfEdge2DNavigator* navigator0 = new HalfEdge2DNavigator(widget_multi);
    HalfEdge2DNavigator* navigator1 = new HalfEdge2DNavigator(widget_single);

    HalfEdge2DController* controller0 = new HalfEdge2DController(widget_multi);
    HalfEdge2DController* controller1 = new HalfEdge2DController(widget_single);

    controller0->setScene(scene);
    controller1->setScene(scene);

    // create renderer
    HalfEdge2DRenderer* renderer0 = new HalfEdge2DRenderer();
    renderer0->setScene(scene);
    renderer0->setWidget(widget_multi);

    HalfEdge2DRenderer* renderer1 = new HalfEdge2DRenderer();
    renderer1->setScene(scene);
    renderer1->setWidget(widget_single);

    // create event handler
    HalfEdge2DEventHandler* eventHandler0 = new HalfEdge2DEventHandler(widget_multi);
    eventHandler0->addEventInterface(navigator0);
    eventHandler0->addEventInterface(controller0);
    eventHandler0->setRenderer(renderer0);

    HalfEdge2DEventHandler* eventHandler1 = new HalfEdge2DEventHandler(widget_single);
    eventHandler1->addEventInterface(navigator1);
    eventHandler1->addEventInterface(controller1);
    eventHandler1->setRenderer(renderer1);

    widget_multi->setEventHandler(eventHandler0);
    widget_multi->addViewPort(viewPort0);
    widget_multi->addViewPort(viewPort1);
    widget_multi->addViewPort(viewPort2);
    widget_multi->addViewPort(viewPort3);

    widget_single->setEventHandler(eventHandler1);
    widget_single->addViewPort(viewPort4);

    // show widget
    main_widget->show();
}
