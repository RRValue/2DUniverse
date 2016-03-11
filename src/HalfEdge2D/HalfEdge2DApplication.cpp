#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/HalfEdge2DWidget.h"
#include "HalfEdge2D/HalfEdge2DEventhandler.h"
#include "HalfEdge2D/HalfEdge2DController.h"
#include "HalfEdge2D/HalfEdge2DNavigator.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/Canvas.h"

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{

}

HalfEdge2DApplication::~HalfEdge2DApplication()
{
    delete m_MainWidget;
    delete m_EventHandler;

    delete m_Navigator;
    delete m_Controller;

    delete m_Scene;
    delete m_Camera;
    delete m_Canvas;
}

void HalfEdge2DApplication::onRun()
{
    init();
}

void HalfEdge2DApplication::init()
{
    // create a scene, camera and canvas
    m_Scene = new Scene();
    m_Canvas = new Canvas();
    m_Camera = new Camera(m_Canvas);

    m_Scene->setCamera(m_Camera);

    // allocate widget
    m_MainWidget = new HalfEdge2DWidget();
    
    // allocate event handler and add controller and navigator
    m_Navigator = new HalfEdge2DNavigator(m_MainWidget);
    m_Controller = new HalfEdge2DController(m_MainWidget);

    // set camera in navigator
    m_Navigator->setCamera(m_Camera);

    // create event handler
    m_EventHandler = new HalfEdge2DEventHandler();
    m_EventHandler->addEventInterface(m_Navigator);
    m_EventHandler->addEventInterface(m_Controller);
    
    m_MainWidget->setEventHandler(m_EventHandler);
    m_MainWidget->show();
}
