#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/Events/EventHandler.h"

#include "HalfEdge2D/Controller/DefaultOptionWidget.h"

#include "HalfEdge2D/Controller/BuildMesh/ControllerBuildMesh.h"
#include "HalfEdge2D/Controller/ShowRings/ControllerShowRings.h"
#include "HalfEdge2D/Controller/Delaunay/ControllerDelaunay.h"
#include "HalfEdge2D/Controller/QuadraticBezier/ControllerQuadraticBezier.h"
#include "HalfEdge2D/Controller/CubicBezier/ControllerCubicBezier.h"
#include "HalfEdge2D/Controller/Spline/ControllerSpline.h"
#include "HalfEdge2D/Controller/SplineAt/ControllerSplineAt.h"
#include "HalfEdge2D/Controller/CutLine/ControllerCutLine.h"
#include "HalfEdge2D/Controller/CutQuadraticBezier/ControllerCutQuadraticBezier.h"
#include "HalfEdge2D/Controller/CutCubicBezier/ControllerCutCubicBezier.h"
#include "HalfEdge2D/Controller/CutCircle/ControllerCutCircle.h"
#include "HalfEdge2D/Controller/CutSpline/ControllerCutSpline.h"
#include "HalfEdge2D/Controller/SplitAt/ControllerSplitAt.h"
#include "HalfEdge2D/Controller/CutMeshLine/ControllerCutMeshLine.h"

#include "HalfEdge2D/Navigation/Navigator.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QWidgetTarget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{
    m_MultiView = false;
    m_HPartition = 0.5f;
    m_VPartition = 0.5f;

    m_CurrentOptionWidget = nullptr;
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
    createController();

    setUpMultiView();

    // show widget
    m_MainWidget->show();

    // set mesh to show
    onControllerSelectionChanged(m_CbController->currentText());
}

void HalfEdge2DApplication::createGui()
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
    connect(m_MainWindowForm.m_CbMultiView, &QCheckBox::stateChanged, this, &HalfEdge2DApplication::onMultiViewChanged);
    connect(m_SldHPart, &QAbstractSlider::valueChanged, this, &HalfEdge2DApplication::onHSliderChanged);
    connect(m_SldVPart, &QAbstractSlider::valueChanged, this, &HalfEdge2DApplication::onVSliderChanged);
    connect(m_CbController, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onControllerSelectionChanged(const QString&)));

    // create default option widget
    m_DefaultOptionWidget = new DefaultOptionWidget();
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

void HalfEdge2DApplication::createController()
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
        new ControllerCutMeshLine()
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

void HalfEdge2DApplication::onMultiViewChanged(int state)
{
    if((Qt::CheckState)state == Qt::Checked)
        m_MultiView = true;
    else
        m_MultiView = false;

    setUpMultiView();
}

void HalfEdge2DApplication::updateViwePortPartions()
{
    m_ViewPort0->setSize(QRectF(0.0f, 0.0f, m_HPartition, m_VPartition));
    m_ViewPort1->setSize(QRectF(m_HPartition, 0.0f, 1.0f - m_HPartition, m_VPartition));
    m_ViewPort2->setSize(QRectF(0.0f, m_VPartition, m_HPartition, 1.0f - m_VPartition));
    m_ViewPort3->setSize(QRectF(m_HPartition, m_VPartition, 1.0f - m_HPartition, 1.0f - m_VPartition));
}

void HalfEdge2DApplication::setUpMultiView()
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

void HalfEdge2DApplication::onHSliderChanged(int value)
{
    m_HPartition = (float)value / 100.0f;

    updateViwePortPartions();

    m_Renderer->render();
}

void HalfEdge2DApplication::onVSliderChanged(int value)
{
    m_VPartition = (float)value / 100.0f;

    updateViwePortPartions();

    m_Renderer->render();
}

void HalfEdge2DApplication::onControllerSelectionChanged(const QString& text)
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
