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

#include "HalfEdge2D/Navigation/Navigator.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QWidgetTarget.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/Camera.h"
#include "HalfEdge2D/Scene/ViewPort.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"

#include "HalfEdge2D/Mesh/TestMesh01.h"
#include "HalfEdge2D/Mesh/TestMesh02.h"
#include "HalfEdge2D/Mesh/TestMesh03.h"

HalfEdge2DApplication::HalfEdge2DApplication(int& argc, char** argv) : QApplication(argc, argv)
{
    m_MultiView = false;
    m_HPartition = 0.5f;
    m_VPartition = 0.5f;

    m_Mesh = new HESMesh();

    m_CurrentOptionWidget = nullptr;
}

HalfEdge2DApplication::~HalfEdge2DApplication()
{
    delete m_Mesh;
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

    // set mesh to show
    onControllerSelectionChanged(m_CbController->currentText());
    onMeshSelectionChanged(m_MainWindowForm.m_CbMeshSelector->currentIndex());
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

    m_MainWindowForm.m_CbMeshSelector->addItems(QStringList() << "Low" << "Mid" << "High" << "Clear");

    m_CbController = m_MainWindowForm.m_CbController;

    m_OptionWidgetContainer = m_MainWindowForm.m_OptionWidgetContainer;
    m_OptionLayout = m_MainWindowForm.m_OptionLayout;

    // connect
    connect(m_MainWindowForm.m_CbMultiView, &QCheckBox::stateChanged, this, &HalfEdge2DApplication::onMultiViewChanged);
    connect(m_SldHPart, &QAbstractSlider::valueChanged, this, &HalfEdge2DApplication::onHSliderChanged);
    connect(m_SldVPart, &QAbstractSlider::valueChanged, this, &HalfEdge2DApplication::onVSliderChanged);
    connect(m_MainWindowForm.m_CbMeshSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onMeshSelectionChanged(int)));
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
    m_Scene = new Scene();

    // allocate event handler and add controller and navigator
    m_Navigator = new Navigator();
    m_ControllerBuildMesh = new ControllerBuildMesh();
    m_ControllerShowRings = new ControllerShowRings();
    m_ControllerDelaunay = new ControllerDelaunay();
    m_ControllerQuadraticBezier = new ControllerQuadraticBezier();
    m_ControllerCubicBezier = new ControllerCubicBezier();
    m_ControllerSpline = new ControllerSpline();
    m_ControllerSplineAt = new ControllerSplineAt();
    m_ControllerCutLine = new ControllerCutLine();
    m_ControllerCutQuadraticBezier = new ControllerCutQuadraticBezier();
    m_ControllerCutCubicBezier = new ControllerCutCubicBezier();
    m_ControllerCutCircle = new ControllerCutCircle();

    m_ControllerBuildMesh->setScene(m_Scene);
    m_ControllerShowRings->setScene(m_Scene);
    m_ControllerDelaunay->setScene(m_Scene);
    m_ControllerQuadraticBezier->setScene(m_Scene);
    m_ControllerCubicBezier->setScene(m_Scene);
    m_ControllerSplineAt->setScene(m_Scene);
    m_ControllerSpline->setScene(m_Scene);
    m_ControllerCutLine->setScene(m_Scene);
    m_ControllerCutQuadraticBezier->setScene(m_Scene);
    m_ControllerCutCubicBezier->setScene(m_Scene);
    m_ControllerCutCircle->setScene(m_Scene);

    // add controller to combobox
    m_CbController->addItem(m_ControllerBuildMesh->getName().c_str());
    m_CbController->addItem(m_ControllerShowRings->getName().c_str());
    m_CbController->addItem(m_ControllerDelaunay->getName().c_str());
    m_CbController->addItem(m_ControllerQuadraticBezier->getName().c_str());
    m_CbController->addItem(m_ControllerCubicBezier->getName().c_str());
    m_CbController->addItem(m_ControllerSpline->getName().c_str());
    m_CbController->addItem(m_ControllerSplineAt->getName().c_str());
    m_CbController->addItem(m_ControllerCutLine->getName().c_str());
    m_CbController->addItem(m_ControllerCutQuadraticBezier->getName().c_str());
    m_CbController->addItem(m_ControllerCutCubicBezier->getName().c_str());
    m_CbController->addItem(m_ControllerCutCircle->getName().c_str());

    m_Controller.insert(std::make_pair(m_ControllerBuildMesh->getName(), m_ControllerBuildMesh));
    m_Controller.insert(std::make_pair(m_ControllerShowRings->getName(), m_ControllerShowRings));
    m_Controller.insert(std::make_pair(m_ControllerDelaunay->getName(), m_ControllerDelaunay));
    m_Controller.insert(std::make_pair(m_ControllerQuadraticBezier->getName(), m_ControllerQuadraticBezier));
    m_Controller.insert(std::make_pair(m_ControllerCubicBezier->getName(), m_ControllerCubicBezier));
    m_Controller.insert(std::make_pair(m_ControllerSpline->getName(), m_ControllerSpline));
    m_Controller.insert(std::make_pair(m_ControllerSplineAt->getName(), m_ControllerSplineAt));
    m_Controller.insert(std::make_pair(m_ControllerCutLine->getName(), m_ControllerCutLine));
    m_Controller.insert(std::make_pair(m_ControllerCutQuadraticBezier->getName(), m_ControllerCutQuadraticBezier));
    m_Controller.insert(std::make_pair(m_ControllerCutCubicBezier->getName(), m_ControllerCutCubicBezier));
    m_Controller.insert(std::make_pair(m_ControllerCutCircle->getName(), m_ControllerCutCircle));

    // create renderer
    m_Renderer = new Renderer();
    m_Renderer->setScene(m_Scene);

    // create event handler
    m_EventHandler = new EventHandler(m_RenderTarget);
    m_EventHandler->setNavigator(m_Navigator);
    m_EventHandler->addController(m_ControllerBuildMesh);
    m_EventHandler->addController(m_ControllerShowRings);
    m_EventHandler->addController(m_ControllerDelaunay);
    m_EventHandler->addController(m_ControllerQuadraticBezier);
    m_EventHandler->addController(m_ControllerCubicBezier);
    m_EventHandler->addController(m_ControllerSpline);
    m_EventHandler->addController(m_ControllerSplineAt);
    m_EventHandler->addController(m_ControllerCutLine);
    m_EventHandler->addController(m_ControllerCutQuadraticBezier);
    m_EventHandler->addController(m_ControllerCutCubicBezier);
    m_EventHandler->addController(m_ControllerCutCircle);
    m_EventHandler->setRenderer(m_Renderer);

    // activate via cb
    m_CbController->setCurrentIndex(0);

    m_RenderTarget->setEventHandler(m_EventHandler);
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

void HalfEdge2DApplication::onMeshSelectionChanged(int value)
{
    m_Scene->removeMesh(m_Mesh);

    m_Mesh->clear();

    if(value < 0 || value >= 3)
    {
        m_ActiveController->setChanged();

        m_Renderer->render();

        return;
    }

    std::vector<float> float_array;
    std::vector<size_t> idx_array;

    if(value == 0)
    {
        float_array = testVertices01;
        idx_array = testTriangles01;
    }

    if(value == 1)
    {
        float_array = testVertices02;
        idx_array = testTriangles02;
    }

    if(value == 2)
    {
        float_array = testVertices03;
        idx_array = testTriangles03;
    }

    for(size_t i = 0; i < float_array.size(); i += 2)
        m_Mesh->addVertex(Vec2f(float_array[i], float_array[i + 1]));

    for(size_t i = 0; i < idx_array.size(); i += 3)
        m_Mesh->addFace({idx_array[i], idx_array[i + 1], idx_array[i + 2]});

    HESBuilder builder(m_Mesh);
    builder.build();

    m_Scene->addMesh(m_Mesh);

    m_ActiveController->setChanged();

    m_Renderer->render();
}

void HalfEdge2DApplication::onControllerSelectionChanged(const QString& text)
{
    std::string name = text.toStdString();

    const auto& find_controller = m_Controller.find(name);

    if(find_controller == m_Controller.end())
        return;

    m_ActiveController = find_controller->second;

    m_EventHandler->setActiveController(m_ActiveController);

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
