#include "2DUniverse/MainApplication.h"

#include "2DUniverse/Events/EventHandler.h"

#include "2DUniverse/Controller/DefaultOptionWidget.h"

#include "2DUniverse/Controller/BuildMesh/ControllerBuildMesh.h"
#include "2DUniverse/Controller/ShowRings/ControllerShowRings.h"
#include "2DUniverse/Controller/Delaunay/ControllerDelaunay.h"
#include "2DUniverse/Controller/QuadraticBezier/ControllerQuadraticBezier.h"
#include "2DUniverse/Controller/CubicBezier/ControllerCubicBezier.h"
#include "2DUniverse/Controller/Spline/ControllerSpline.h"
#include "2DUniverse/Controller/SplineAt/ControllerSplineAt.h"
#include "2DUniverse/Controller/CutLine/ControllerCutLine.h"
#include "2DUniverse/Controller/CutQuadraticBezier/ControllerCutQuadraticBezier.h"
#include "2DUniverse/Controller/CutCubicBezier/ControllerCutCubicBezier.h"
#include "2DUniverse/Controller/CutCircle/ControllerCutCircle.h"
#include "2DUniverse/Controller/CutSpline/ControllerCutSpline.h"
#include "2DUniverse/Controller/SplitAt/ControllerSplitAt.h"
#include "2DUniverse/Controller/CutMesh/ControllerCutMesh.h"

#include "2DUniverse/Navigation/Navigator.h"

#include "2DUniverse/Rendering/Renderer.h"
#include "2DUniverse/Rendering/QWidgetTarget.h"

#include "2DUniverse/Scene/Scene.h"
#include "2DUniverse/Scene/Camera.h"
#include "2DUniverse/Scene/ViewPort.h"

MainApplication::MainApplication(int& argc, char** argv) : QApplication(argc, argv)
{
    m_MultiView = false;
    m_HPartition = 0.5f;
    m_VPartition = 0.5f;

    m_CurrentOptionWidget = nullptr;
}

MainApplication::~MainApplication()
{

}

void MainApplication::onRun()
{
    init();
}

void MainApplication::init()
{
    createGui();
    createViewPorts();
    createRendering();
    createController();

    setUpMultiView();

    // show widget
    m_MainWidget->show();

    // set mesh to show
    onControllerSelectionChanged(m_CbController->currentText());
}

void MainApplication::createGui()
{
    m_MainWidget = new QWidget();
    m_MainWindowForm.setupUi(m_MainWidget);

    m_RenderTarget = new QWidgetTarget(m_MainWindowForm.m_RenderWidget);
    m_SldHPart = m_MainWindowForm.m_SldHPart;
    m_SldVPart = m_MainWindowForm.m_SldVPart;

    m_SldHPart->setEnabled(m_MultiView);
    m_SldVPart->setEnabled(m_MultiView);

    m_CbController = m_MainWindowForm.m_CbController;

    m_OptionWidgetContainer = m_MainWindowForm.m_OptionWidgetContainer;
    m_OptionLayout = m_MainWindowForm.m_OptionLayout;

    // connect
    connect(m_MainWindowForm.m_CbMultiView, &QCheckBox::stateChanged, this, &MainApplication::onMultiViewChanged);
    connect(m_SldHPart, &QAbstractSlider::valueChanged, this, &MainApplication::onHSliderChanged);
    connect(m_SldVPart, &QAbstractSlider::valueChanged, this, &MainApplication::onVSliderChanged);
    connect(m_CbController, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onControllerSelectionChanged(const QString&)));

    // create default option widget
    m_DefaultOptionWidget = new DefaultOptionWidget();
}

void MainApplication::createViewPorts()
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

void MainApplication::createRendering()
{
    // allocate event handler and add controller and navigator
    m_Navigator = new Navigator();

    // create renderer
    m_Renderer = new Renderer();

    // create event handler
    m_EventHandler = new EventHandler(m_RenderTarget);
    m_EventHandler->setNavigator(m_Navigator);

    // set render in event handler
    m_EventHandler->setRenderer(m_Renderer);

    // set event handler in render target
    m_RenderTarget->setEventHandler(m_EventHandler);
}

void MainApplication::createController()
{
    std::vector<Controller*> controller_to_add =
    {
        new ControllerBuildMesh(),
        new ControllerShowRings(),
        new ControllerDelaunay(),
        new ControllerQuadraticBezier(),
        new ControllerCubicBezier(),
        new ControllerSpline(),
        new ControllerSplineAt(),
        new ControllerCutLine(),
        new ControllerCutQuadraticBezier(),
        new ControllerCutCubicBezier(),
        new ControllerCutCircle(),
        new ControllerCutSpline(),
        new ControllerSplitAt(),
        new ControllerCutMesh()
    };

    for(const auto& c : controller_to_add)
        m_Controller.insert(std::make_pair(c->getName(), c));

    // add controller to combobox
    m_CbController->blockSignals(true);

    for(const auto& c : controller_to_add)
        m_CbController->addItem(c->getName().c_str());

    m_CbController->blockSignals(false);

    // add controller to event handler
    for(const auto& c : controller_to_add)
        m_EventHandler->addController(c);

    // init controller
    for(const auto& c : controller_to_add)
        c->init();
}

void MainApplication::onMultiViewChanged(int state)
{
    if((Qt::CheckState)state == Qt::Checked)
        m_MultiView = true;
    else
        m_MultiView = false;

    setUpMultiView();
}

void MainApplication::updateViwePortPartions()
{
    m_ViewPort0->setSize(QRectF(0.0f, 0.0f, m_HPartition, m_VPartition));
    m_ViewPort1->setSize(QRectF(m_HPartition, 0.0f, 1.0f - m_HPartition, m_VPartition));
    m_ViewPort2->setSize(QRectF(0.0f, m_VPartition, m_HPartition, 1.0f - m_VPartition));
    m_ViewPort3->setSize(QRectF(m_HPartition, m_VPartition, 1.0f - m_HPartition, 1.0f - m_VPartition));
}

void MainApplication::setUpMultiView()
{
    m_RenderTarget->clearViewPorts();

    if(m_MultiView)
    {
        updateViwePortPartions();

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

    m_SldHPart->setEnabled(m_MultiView);
    m_SldVPart->setEnabled(m_MultiView);

    m_Renderer->render();
}

void MainApplication::onHSliderChanged(int value)
{
    m_HPartition = (float)value / 100.0f;

    updateViwePortPartions();

    m_Renderer->render();
}

void MainApplication::onVSliderChanged(int value)
{
    m_VPartition = (float)value / 100.0f;

    updateViwePortPartions();

    m_Renderer->render();
}

void MainApplication::onControllerSelectionChanged(const QString& text)
{
    std::string name = text.toStdString();

    const auto& find_controller = m_Controller.find(name);

    if(find_controller == m_Controller.end())
        return;

    m_ActiveController = find_controller->second;

    // set active controller in event handler
    m_EventHandler->setActiveController(m_ActiveController);

    // set scene from active controller
    m_Renderer->setScene(m_ActiveController->getScene());

    // add controller widget
    QWidget* option_widget = m_ActiveController->getOptionWidget();

    if(option_widget == nullptr)
        option_widget = m_DefaultOptionWidget;

    if(m_CurrentOptionWidget != nullptr)
    {
        m_OptionLayout->removeWidget(m_CurrentOptionWidget);
        m_CurrentOptionWidget->setParent(nullptr);
    }

    m_CurrentOptionWidget = option_widget;

    m_CurrentOptionWidget->setParent(m_OptionWidgetContainer);
    m_OptionLayout->addWidget(m_CurrentOptionWidget);
}
