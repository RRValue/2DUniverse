#include "HalfEdge2D/Controller/CutMeshLine/ControllerCutMeshLine.h"

#include "HalfEdge2D/Controller/CutMeshLine/CutMeshLineOption_uic.h"

#include "HalfEdge2D/Base/Spline.h"

#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"
#include "HalfEdge2D/HalfEdge/HESFace.h"
#include "HalfEdge2D/HalfEdge/HESBuilder.h"
#include "HalfEdge2D/HalfEdge/HESCutter.h"
#include "HalfEdge2D/HalfEdge/HESCheck.h"

#include "HalfEdge2D/Mesh/TestMesh01.h"
#include "HalfEdge2D/Mesh/TestMesh02.h"
#include "HalfEdge2D/Mesh/TestMesh03.h"
#include "HalfEdge2D/Mesh/TestMesh04.h"
#include "HalfEdge2D/Mesh/TestMesh05.h"
#include "HalfEdge2D/Mesh/TestMesh06.h"

#include <QtGui/QMouseEvent>

ControllerCutMeshLine::ControllerCutMeshLine()
{
    m_Name = "ControllerCutMeshLine";
}

ControllerCutMeshLine::~ControllerCutMeshLine()
{
    for(auto& m : m_Meshes)
        delete m;

    for(auto& m : m_CutMeshes)
        delete m;

    for(const auto& p : m_Points)
        delete p;

    delete m_Line;

    delete m_MeshCutter;
    delete m_MeshBuilder;
}

void ControllerCutMeshLine::init()
{
    m_MeshCutter = new HESCutter();
    m_MeshBuilder = new HESBuilder();
    m_MeshChecker = new HESCheck();

    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Line = new Line();
    m_Line->setVisible(false);
    m_Line->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));

    // add to scene
    m_Scene->addLine(m_Line);

    // init gui
    m_OptionWidget = new QWidget();

    Ui_ControllerCutMeshLineOptionsWidget ui_options;
    ui_options.setupUi(m_OptionWidget);

    m_CbMeshSelector = ui_options.m_CbMeshSelector;
    m_CbMeshSelector->addItems(QStringList() << "Low" << "Mid" << "High" << "Parts" << "Parts2" << "Hole" << "Clear");

    connect(m_CbMeshSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onMeshSelectionChanged(int)));

    onMeshSelectionChanged(m_CbMeshSelector->currentIndex());
}

void ControllerCutMeshLine::activate()
{
    m_Renderer->setRenderCoordianteAxis(false);
    m_Renderer->setRenderTriangles(false);
    m_Renderer->setRenderVertices(false);
}

void ControllerCutMeshLine::deactivate()
{
    m_Renderer->setRenderCoordianteAxis(true);
    m_Renderer->setRenderTriangles(true);
    m_Renderer->setRenderVertices(true);
}

bool ControllerCutMeshLine::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i pos_in_vp = keepInViewPort(pos_global);
    Vec2f pos = Vec2f((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2f new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);

    if(m_CurrentPointIdx == 0)
        m_Line->setPoint(0, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 1)
        m_Line->setPoint(1, m_CurrentPoint->getPosition());

    cut();

    m_Renderer->render();

    return true;
}

bool ControllerCutMeshLine::handleMousePressEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(m_MovePoint)
        return false;

    if(m_ActiveViewPort == nullptr)
        return false;

    if(m_ActiveCamera == nullptr)
        return false;

    updateTransMatrix();

    // set to move mode
    if(event->button() == Qt::LeftButton)
        m_MovePoint = true;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i p = keepInViewPort(pos_global);
    Vec2f p_f(p[0], p[1]);

    m_CurrentPoint = getPointAtPos(invTrans(p_f), &m_CurrentPointIdx);

    // if hit nothing and point size < 2 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 2)
        {
            m_MovePoint = false;

            return true;
        }

        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));

        m_CurrentPointIdx = m_Points.size();

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_CurrentPointIdx == 0)
            m_Line->setPoint(0, m_CurrentPoint->getPosition());

        if(m_CurrentPointIdx == 1)
        {
            m_Line->setPoint(1, m_CurrentPoint->getPosition());
            m_Line->setVisible(true);
        }
    }

    cut();

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCutMeshLine::handleMouseReleaseEvent(QMouseEvent* const event)
{
    if(m_Scene == nullptr)
        return false;

    if(event == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    m_MovePoint = false;

    return true;
}

bool ControllerCutMeshLine::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutMeshLine::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCutMeshLine::getPointAtPos(const Vec2f& pos, size_t* const idx) const
{
    if(m_Points.empty())
        return nullptr;

    for(size_t i = 0; i < m_Points.size(); i++)
    {
        if((m_Points[i]->getPosition() - pos).norm() >= m_Points[i]->getSize())
            continue;

        *idx = i;

        return m_Points[i];
    }

    return nullptr;
}

void ControllerCutMeshLine::onMeshSelectionChanged(int value)
{
    HESMesh tmp_mesh;

    if(value < 0 || value >= 6)
    {
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

    if(value == 3)
    {
        float_array = testVertices04;
        idx_array = testTriangles04;
    }

    if(value == 4)
    {
        float_array = testVertices05;
        idx_array = testTriangles05;
    }

    if(value == 5)
    {
        float_array = testVertices06;
        idx_array = testTriangles06;
    }

    for(size_t i = 0; i < float_array.size(); i += 2)
        tmp_mesh.addVertex(Vec2f(float_array[i], float_array[i + 1]));

    for(size_t i = 0; i < idx_array.size(); i += 3)
        tmp_mesh.addFace({idx_array[i], idx_array[i + 1], idx_array[i + 2]});

    m_MeshBuilder->build(&tmp_mesh);
    m_MeshChecker->run(&tmp_mesh);

    // remove old meshes from scene
    for(auto& m : m_Meshes)
    {
        m_Scene->removeMesh(m);
        delete m;
    }

    m_Meshes = m_MeshChecker->getMeshes();

    for(auto& m : m_Meshes)
        m_Scene->addMesh(m);

    m_SceneChanges = true;

    m_Renderer->render();
}

void ControllerCutMeshLine::cut()
{
    if(m_Points.size() != 2)
        return;

    // clear and remove cut points
    for(const auto& p : m_CutPoints)
    {
        m_Scene->removePoint(p);
        delete p;
    }

    m_CutPoints.clear();

    NSpline<float, 4> rgbg;

    rgbg.addPoint(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    rgbg.addPoint(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    rgbg.addPoint(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    rgbg.setClosed(false);

    // for each mesh cut
    for(const auto& m : m_Meshes)
    {
        HESMeshVector result;
        bool cutted = m_MeshCutter->cutLine(m, m_Line, result);

        if(!cutted)
            continue;

        // add cut points
        const CutPointVector& cp_vec = m_MeshCutter->getCutPoints();
        const size_t& num_cp = cp_vec.size();
        
        float s = 1.0f / (float)(num_cp - 1);
        float t = 0.0f;

        for(size_t i = 0; i < num_cp; i++)
        {
            Point* cp = new Point();
            cp->setPosition(cp_vec[i]);
            cp->setColour(rgbg.pointAt(t));
            cp->setSize(0.02f);

            m_CutPoints.push_back(cp);

            t += s;
        }
    }

    // add cut points to scene
    for(const auto& p : m_CutPoints)
        m_Scene->addPoint(p);
}
