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
    m_MultiView = false;
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
    createGui();
    createViewPorts();
    createRendering();

    setUpMultiView();

    // show widget
    m_MainWidget->show();
}

void HalfEdge2DApplication::createGui()
{
    m_MainWidget = new QWidget();
    m_MainWindowForm.setupUi(m_MainWidget);

    m_RenderTarget = m_MainWindowForm.m_RenderWidget;

    // connect
    connect(m_MainWindowForm.m_CbMultiView, &QCheckBox::stateChanged, this, &HalfEdge2DApplication::onMultiViewChanged);
}

void HalfEdge2DApplication::createViewPorts()
{
    m_CamVp0 = new Camera();
    m_CamVp1 = new Camera();
    m_CamVp2 = new Camera();
    m_CamVp3 = new Camera();

    m_ViewPort0 = new ViewPort();
    m_ViewPort0->setCamera(m_CamVp0);

    m_ViewPort1 = new ViewPort();
    m_ViewPort1->setCamera(m_CamVp1);

    m_ViewPort2 = new ViewPort();
    m_ViewPort2->setCamera(m_CamVp2);

    m_ViewPort3 = new ViewPort();
    m_ViewPort3->setCamera(m_CamVp3);
}

void HalfEdge2DApplication::createRendering()
{
    Scene* scene = new Scene();

    // allocate event handler and add controller and navigator
    Navigator* navigator = new Navigator();
    Controller* controller = new Controller();

    controller->setScene(scene);

    // create renderer
    m_Renderer = new Renderer();
    m_Renderer->setScene(scene);

    // create event handler
    EventHandler* eventHandler = new EventHandler(m_RenderTarget);
    eventHandler->addEventInterface(navigator);
    eventHandler->addEventInterface(controller);
    eventHandler->setRenderer(m_Renderer);

    m_RenderTarget->setEventHandler(eventHandler);
}

void HalfEdge2DApplication::onMultiViewChanged(int state)
{
    if((Qt::CheckState)state == Qt::Checked)
        m_MultiView = true;
    else
        m_MultiView = false;

    setUpMultiView();
}

void HalfEdge2DApplication::setUpMultiView()
{
    m_RenderTarget->clearViewPorts();

    if(m_MultiView)
    {
        m_ViewPort0->setSize(QRectF(0.0f, 0.0f, 0.5f, 0.5f));
        m_ViewPort1->setSize(QRectF(0.5f, 0.0f, 0.5f, 0.5f));
        m_ViewPort2->setSize(QRectF(0.0f, 0.5f, 0.5f, 0.5f));
        m_ViewPort3->setSize(QRectF(0.5f, 0.5f, 0.5f, 0.5f));

        m_RenderTarget->addViewPort(m_ViewPort0);
        m_RenderTarget->addViewPort(m_ViewPort1);
        m_RenderTarget->addViewPort(m_ViewPort2);
        m_RenderTarget->addViewPort(m_ViewPort3);
    }
    else
    {
        m_ViewPort0->setSize(QRectF(0.0f, 0.0f, 1.0f, 1.0f));

        m_RenderTarget->addViewPort(m_ViewPort0);
    }

    m_Renderer->render();
}
