#include "HalfEdge2D/HalfEdge2DApplication.h"

#include "HalfEdge2D/Events/EventHandler.h"

#include "HalfEdge2D/Controlling/ControllerBuildMesh.h"
#include "HalfEdge2D/Controlling/ControllerShowRings.h"

#include "HalfEdge2D/Navigation/Navigator.h"

#include "HalfEdge2D/Rendering/Renderer.h"
#include "HalfEdge2D/Rendering/QPaintTarget.h"

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
    onMeshSelectionChanged(m_MainWindowForm.m_CbMeshSelector->currentIndex());
}

void HalfEdge2DApplication::createGui()
{
    m_MainWidget = new QWidget();
    m_MainWindowForm.setupUi(m_MainWidget);

    m_RenderTarget = m_MainWindowForm.m_RenderWidget;
    m_SldHPart = m_MainWindowForm.m_SldHPart;
    m_SldVPart = m_MainWindowForm.m_SldVPart;

    m_SldHPart->setEnabled(m_MultiView);
    m_SldVPart->setEnabled(m_MultiView);

    m_MainWindowForm.m_CbMeshSelector->addItems(QStringList() << "Low" << "Mid" << "High" << "Clear");

    m_CbController = m_MainWindowForm.m_CbController;

    // connect
    connect(m_MainWindowForm.m_CbMultiView, &QCheckBox::stateChanged, this, &HalfEdge2DApplication::onMultiViewChanged);
    connect(m_SldHPart, &QAbstractSlider::valueChanged, this, &HalfEdge2DApplication::onHSliderChanged);
    connect(m_SldVPart, &QAbstractSlider::valueChanged, this, &HalfEdge2DApplication::onVSliderChanged);
    connect(m_MainWindowForm.m_CbMeshSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onMeshSelectionChanged(int)));
    connect(m_CbController, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onControllerSelectionChanged(const QString&)));
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

    m_Scene->setMesh(m_Mesh);

    // allocate event handler and add controller and navigator
    m_Navigator = new Navigator();
    m_ControllerBuildMesh = new ControllerBuildMesh();
    m_ControllerShowRings = new ControllerShowRings();

    m_ControllerBuildMesh->setMesh(m_Mesh);
    m_ControllerShowRings->setMesh(m_Mesh);

    // add controller to combobox
    m_CbController->addItem(m_ControllerBuildMesh->getName().c_str());
    m_CbController->addItem(m_ControllerShowRings->getName().c_str());

    m_Controller.insert(std::make_pair(m_ControllerBuildMesh->getName(), m_ControllerBuildMesh));
    m_Controller.insert(std::make_pair(m_ControllerShowRings->getName(), m_ControllerShowRings));

    // create renderer
    m_Renderer = new Renderer();
    m_Renderer->setScene(m_Scene);

    // create event handler
    m_EventHandler = new EventHandler(m_RenderTarget);
    m_EventHandler->setNavigator(m_Navigator);
    m_EventHandler->addController(m_ControllerBuildMesh);
    m_EventHandler->addController(m_ControllerShowRings);
    m_EventHandler->setRenderer(m_Renderer);
    m_EventHandler->setActiveController(m_ControllerBuildMesh);

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
    m_Mesh->clear();

    if(value < 0 || value >= 3)
    {
        m_Renderer->render();

        return;
    }

    std::vector<float> float_array;
    std::vector<int> idx_array;

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
        m_Mesh->addTriangle(idx_array[i], idx_array[i + 1], idx_array[i + 2]);

    HESBuilder builder(m_Mesh);
    builder.build();

    m_Renderer->render();
}

void HalfEdge2DApplication::onControllerSelectionChanged(const QString& text)
{
    std::string name = text.toStdString();

    const auto& find_controller = m_Controller.find(name);

    if(find_controller == m_Controller.end())
        return;

    m_EventHandler->setActiveController(find_controller->second);
}
