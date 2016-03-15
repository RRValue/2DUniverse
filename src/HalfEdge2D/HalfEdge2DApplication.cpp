#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/Events/EventHandler.h"

#include "HalfEdge2D/Controlling/Controller.h"

#include "HalfEdge2D/Navigation/Navigator.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"

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
    initTest();
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
    Navigator* navigator = new Navigator();
    Controller* controller = new Controller();

    controller->setScene(scene);

    // create renderer
    Renderer* renderer = new Renderer();
    renderer->setScene(scene);

    // create event handler
    EventHandler* eventHandler = new EventHandler(paintTarget);
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
    m_MainWindowForm.setupUi(main_widget);

    // get paint target
    QPaintTarget* widget_multi = m_MainWindowForm.m_RenderWidget0;
    QPaintTarget* widget_single = m_MainWindowForm.m_RenderWidget1;

    // init scene
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
    Navigator* navigator = new Navigator();
    Controller* controller = new Controller();

    controller->setScene(scene);

    // create renderer
    Renderer* renderer = new Renderer();
    renderer->setScene(scene);

    // create event handler
    EventHandler* eventHandler_multi = new EventHandler(widget_multi);
    eventHandler_multi->addEventInterface(navigator);
    eventHandler_multi->addEventInterface(controller);
    eventHandler_multi->setRenderer(renderer);

    EventHandler* eventHandler_single = new EventHandler(widget_single);
    eventHandler_single->addEventInterface(navigator);
    eventHandler_single->addEventInterface(controller);
    eventHandler_single->setRenderer(renderer);

    widget_multi->setEventHandler(eventHandler_multi);
    widget_multi->addViewPort(viewPort0);
    widget_multi->addViewPort(viewPort1);
    widget_multi->addViewPort(viewPort2);
    widget_multi->addViewPort(viewPort3);

    widget_single->setEventHandler(eventHandler_single);
    widget_single->addViewPort(viewPort4);

    // show widget
    main_widget->show();
}
