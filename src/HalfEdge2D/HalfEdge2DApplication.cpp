#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/HalfEdge2DEventhandler.h"
#include "HalfEdge2D/HalfEdge2DController.h"
#include "HalfEdge2D/HalfEdge2DNavigator.h"
#include "HalfEdge2D/HalfEdge2DRenderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/Canvas.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/QPaintTarget.h"

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{

}

HalfEdge2DApplication::~HalfEdge2DApplication()
{
    delete m_PaintTarget;
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

    m_ViewPort0 = new ViewPort();
    m_ViewPort0->setSize(QRectF(0.0f, 0.0f, 0.5f, 0.5f));
    m_ViewPort0->setCamera(m_Camera);

    m_ViewPort1 = new ViewPort();
    m_ViewPort1->setSize(QRectF(0.5f, 0.0f, 0.5f, 0.5f));
    m_ViewPort1->setCamera(m_Camera);

    m_ViewPort2 = new ViewPort();
    m_ViewPort2->setSize(QRectF(0.0f, 0.5f, 0.5f, 0.5f));
    m_ViewPort2->setCamera(m_Camera);

    m_ViewPort3 = new ViewPort();
    m_ViewPort3->setSize(QRectF(0.5f, 0.5f, 0.5f, 0.5f));
    m_ViewPort3->setCamera(m_Camera);

    m_Scene->setCamera(m_Camera);

    // allocate widget
    m_PaintTarget = new QPaintTarget();
    
    // allocate event handler and add controller and navigator
    m_Navigator = new HalfEdge2DNavigator(m_PaintTarget);
    m_Controller = new HalfEdge2DController(m_PaintTarget);

    // set camera in navigator
    m_Navigator->setScene(m_Scene);
    m_Controller->setScene(m_Scene);

    // create renderer
    m_Renderer = new HalfEdge2DRenderer();
    m_Renderer->setScene(m_Scene);
    m_Renderer->setWidget(m_PaintTarget);

    // create event handler
    m_EventHandler = new HalfEdge2DEventHandler(m_PaintTarget);
    m_EventHandler->addEventInterface(m_Navigator);
    m_EventHandler->addEventInterface(m_Controller);
    m_EventHandler->setRenderer(m_Renderer);
    
    m_PaintTarget->setEventHandler(m_EventHandler);
    m_PaintTarget->addViewPort(m_ViewPort0);
    m_PaintTarget->addViewPort(m_ViewPort1);
    m_PaintTarget->addViewPort(m_ViewPort2);
    m_PaintTarget->addViewPort(m_ViewPort3);
    m_PaintTarget->show();
}
