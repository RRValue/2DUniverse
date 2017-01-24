#include "HalfEdge2D/Controller/CutMesh/ControllerCutMesh.h"

#include "HalfEdge2D/Controller/CutMesh/CutMeshOption_uic.h"

#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"
#include "HalfEdge2D/Renderables/QuadraticBezier.h"
#include "HalfEdge2D/Renderables/CubicBezier.h"
#include "HalfEdge2D/Renderables/Spline.h"

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
#include "HalfEdge2D/Mesh/TestMesh07.h"

#include <QtGui/QMouseEvent>

ControllerCutMesh::ControllerCutMesh()
{
    m_Name = "ControllerCutMesh";
    m_CutMode = CMM_LINE;
}

ControllerCutMesh::~ControllerCutMesh()
{
    for(auto& m : m_Meshes)
        delete m;

    for(auto& m : m_CutMeshes)
        delete m;

    for(const auto& p : m_LinePoints)
        delete p;

    for(const auto& p : m_QBezierPoints)
        delete p;

    for(const auto& p : m_CBezierPoints)
        delete p;

    for(const auto& p : m_SplinePoints)
        delete p;

    delete m_Line;
    delete m_QBezier;
    delete m_CBezier;
    delete m_Spline;

    delete m_MeshCutter;
    delete m_MeshBuilder;
}

void ControllerCutMesh::init()
{
    m_MeshCutter = new HESCutter();
    m_MeshBuilder = new HESBuilder();
    m_MeshChecker = new HESCheck();

    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Line = new Line();
    m_Line->setVisible(false);
    m_Line->setColour(Vec4f(0.33f, 0.33f, 0.33f, 0.333f));

    m_QBezier = new QuadraticBezier();
    m_QBezier->setVisible(false);
    m_QBezier->setColour(Vec4f(0.33f, 0.33f, 0.33f, 0.333f));

    m_CBezier = new CubicBezier();
    m_CBezier->setVisible(false);
    m_CBezier->setColour(Vec4f(0.33f, 0.33f, 0.33f, 0.333f));

    m_Spline = new Spline();
    m_Spline->setVisible(false);
    m_Spline->setColour(Vec4f(0.33f, 0.33f, 0.33f, 0.333f));

    // add to scene
    m_Scene->addLine(m_Line);
    m_Scene->addQuadraticBeziers(m_QBezier);
    m_Scene->addCubicBeziers(m_CBezier);
    m_Scene->addSpline(m_Spline);

    // init gui
    m_OptionWidget = new QWidget();

    Ui_ControllerCutMeshOptionsWidget ui_options;
    ui_options.setupUi(m_OptionWidget);

    m_CbMeshSelector = ui_options.m_CbMeshSelector;
    m_CbMeshSelector->addItems(QStringList() << "Low" << "Mid" << "High" << "Parts" << "Parts2" << "Hole" << "Triangle" << "Clear");

    connect(m_CbMeshSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onMeshSelectionChanged(int)));

    onMeshSelectionChanged(m_CbMeshSelector->currentIndex());

    m_CbShapeSelector = ui_options.m_CbShapeSelector;
    m_CbShapeSelector->addItems(QStringList() << "Line" << "QBezier" << "CBezier" << "SPline");

    connect(m_CbShapeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onShapeSelectionChanged(int)));

    connect(ui_options.m_cbSmallTrianglePrevention, &QCheckBox::stateChanged, this, &ControllerCutMesh::onSmallTrianglePreventionChanged);
    connect(ui_options.m_BtnCut, &QPushButton::pressed, this, &ControllerCutMesh::onCutPressed);
}

void ControllerCutMesh::activate()
{
    m_Renderer->setRenderCoordianteAxis(false);
    m_Renderer->setRenderTriangles(false);
    m_Renderer->setRenderVertices(true);
}

void ControllerCutMesh::deactivate()
{
    m_Renderer->setRenderCoordianteAxis(true);
    m_Renderer->setRenderTriangles(true);
    m_Renderer->setRenderVertices(true);
}

bool ControllerCutMesh::handleMouseMoveEvent(QMouseEvent* const event)
{
    if(m_RenderTarget == nullptr || m_ActiveViewPort == nullptr || m_ActiveCamera == nullptr)
        return false;

    if(m_Scene == nullptr)
        return false;

    if(!m_MovePoint)
        return false;

    Vec2i pos_global(event->pos().x(), event->pos().y());
    Vec2i pos_in_vp = keepInViewPort(pos_global);
    Vec2d pos = Vec2d((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2d new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);

    switch(m_CutMode)
    {
    case CMM_LINE:
        {
            m_Line->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

            break;
        }
    case CMM_QBEZIER:
        {
            m_QBezier->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

            break;
        }
    case CMM_CBEZIER:
        {
            m_CBezier->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

            break;
        }
    case CMM_SPLINE:
        {
            m_Spline->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

            break;
        }
    }

    cut();

    m_Renderer->render();

    return true;
}

bool ControllerCutMesh::handleMousePressEvent(QMouseEvent* const event)
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
    Vec2d p_f(p[0], p[1]);

    m_CurrentPoint = getPointAtPos(invTrans(p_f), &m_CurrentPointIdx);

    // if hit nothing and point size < 2 -> add
    if(m_CurrentPoint == nullptr)
    {
        switch(m_CutMode)
        {
        case CMM_LINE:
            {
                if(m_LinePoints.size() >= m_NumLinePoints)
                {
                    m_MovePoint = false;

                    return true;
                }

                break;
            }
        case CMM_QBEZIER:
            {
                if(m_QBezierPoints.size() >= m_NumQBezierPoints)
                {
                    m_MovePoint = false;

                    return true;
                }

                break;
            }
        case CMM_CBEZIER:
            {
                if(m_CBezierPoints.size() >= m_NumCBezierPoints)
                {
                    m_MovePoint = false;

                    return true;
                }

                break;
            }
        case CMM_SPLINE:
            {
                if(m_SplinePoints.size() >= m_NumSplinePoints)
                {
                    m_MovePoint = false;

                    return true;
                }

                break;
            }
        default:
            {
                m_MovePoint = false;

                return true;
            }
        }

        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));

        switch(m_CutMode)
        {
        case CMM_LINE:
            {
                m_CurrentPointIdx = m_LinePoints.size();

                m_LinePoints.push_back(m_CurrentPoint);
                m_Scene->addPoint(m_CurrentPoint);

                if(m_CurrentPointIdx == 0)
                    m_Line->setPoint(0, m_CurrentPoint->getPosition());

                if(m_CurrentPointIdx == 1)
                {
                    m_Line->setPoint(1, m_CurrentPoint->getPosition());
                    m_Line->setVisible(true);
                }

                break;
            }
        case CMM_QBEZIER:
            {
                m_CurrentPointIdx = m_QBezierPoints.size();

                m_QBezierPoints.push_back(m_CurrentPoint);
                m_Scene->addPoint(m_CurrentPoint);

                if(m_CurrentPointIdx == 0)
                    m_QBezier->setPoint(0, m_CurrentPoint->getPosition());

                if(m_CurrentPointIdx == 1)
                    m_QBezier->setPoint(1, m_CurrentPoint->getPosition());

                if(m_CurrentPointIdx == 2)
                {
                    m_QBezier->setPoint(2, m_CurrentPoint->getPosition());
                    m_QBezier->setVisible(true);
                }

                break;
            }
        case CMM_CBEZIER:
            {
                m_CurrentPointIdx = m_CBezierPoints.size();

                m_CBezierPoints.push_back(m_CurrentPoint);
                m_Scene->addPoint(m_CurrentPoint);

                if(m_CurrentPointIdx == 0)
                    m_CBezier->setPoint(0, m_CurrentPoint->getPosition());

                if(m_CurrentPointIdx == 1)
                    m_CBezier->setPoint(1, m_CurrentPoint->getPosition());

                if(m_CurrentPointIdx == 2)
                    m_CBezier->setPoint(2, m_CurrentPoint->getPosition());

                if(m_CurrentPointIdx == 3)
                {
                    m_CBezier->setPoint(3, m_CurrentPoint->getPosition());
                    m_CBezier->setVisible(true);
                }

                break;
            }
        case CMM_SPLINE:
            {
                m_CurrentPointIdx = m_SplinePoints.size();

                m_SplinePoints.push_back(m_CurrentPoint);
                m_Scene->addPoint(m_CurrentPoint);

                m_Spline->addPoint(m_CurrentPoint->getPosition());

                if(m_SplinePoints.size() == 2)
                    m_Spline->setVisible(true);

                if(m_SplinePoints.size() > 2)
                    m_Spline->setClosed(true);

                break;
            }
        default:
            break;
        }
    }

    cut();

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCutMesh::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerCutMesh::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutMesh::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCutMesh::getPointAtPos(const Vec2d& pos, size_t* const idx) const
{
    const std::vector<Point*>* current_point_list = nullptr;

    switch(m_CutMode)
    {
    case CMM_LINE:
        {
            current_point_list = &m_LinePoints;

            break;
        }
    case CMM_QBEZIER:
        {
            current_point_list = &m_QBezierPoints;

            break;
        }
    case CMM_CBEZIER:
        {
            current_point_list = &m_CBezierPoints;

            break;
        }
    case CMM_SPLINE:
        {
            current_point_list = &m_SplinePoints;

            break;
        }
    default:
        return nullptr;
    }

    if(current_point_list == nullptr)
        return nullptr;

    if(current_point_list->empty())
        return nullptr;

    for(size_t i = 0; i < current_point_list->size(); i++)
    {
        if(((*current_point_list)[i]->getPosition() - pos).norm() >= (*current_point_list)[i]->getSize())
            continue;

        *idx = i;

        return (*current_point_list)[i];
    }

    return nullptr;
}

void ControllerCutMesh::onMeshSelectionChanged(int value)
{
    HESMesh tmp_mesh;

    if(value < 0 || value >= 7)
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

    if(value == 6)
    {
        float_array = testVertices07;
        idx_array = testTriangles07;
    }

    for(size_t i = 0; i < float_array.size(); i += 2)
        tmp_mesh.addVertex(Vec2d(float_array[i], float_array[i + 1]));

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

    cut();

    m_Renderer->render();
}

void ControllerCutMesh::onSmallTrianglePreventionChanged(int state)
{
    m_MeshCutter->enableSmallTrianglePrevention(state == Qt::Checked);

    cut();

    m_Renderer->render();
}

void ControllerCutMesh::onShapeSelectionChanged(int value)
{
    if(value < 0 || value > 3)
        return;

    bool show_line_points = value == 0;
    bool show_qbez_points = value == 1;
    bool show_cbez_points = value == 2;
    bool show_spli_points = value == 3;

    bool show_line = show_line_points && m_LinePoints.size() == m_NumLinePoints;
    bool show_qbez = show_qbez_points && m_QBezierPoints.size() == m_NumQBezierPoints;
    bool show_cbez = show_cbez_points && m_CBezierPoints.size() == m_NumCBezierPoints;
    bool show_spli = show_spli_points && m_SplinePoints.size() > 1;

    for(const auto& p : m_LinePoints)
        p->setVisible(show_line_points);
    m_Line->setVisible(show_line);

    for(const auto& p : m_QBezierPoints)
        p->setVisible(show_qbez_points);
    m_QBezier->setVisible(show_qbez);

    for(const auto& p : m_CBezierPoints)
        p->setVisible(show_cbez_points);
    m_CBezier->setVisible(show_cbez);

    for(const auto& p : m_SplinePoints)
        p->setVisible(show_spli_points);
    m_Spline->setVisible(show_spli);

    if(show_line_points)
        m_CutMode = CMM_LINE;

    if(show_qbez_points)
        m_CutMode = CMM_QBEZIER;

    if(show_cbez_points)
        m_CutMode = CMM_CBEZIER;

    if(show_spli_points)
        m_CutMode = CMM_SPLINE;

    cut();

    m_Renderer->render();
}

void ControllerCutMesh::cut()
{
    // clear and remove cut points
    for(const auto& p : m_CutPoints)
    {
        m_Scene->removePoint(p);
        delete p;
    }

    m_CutPoints.clear();

    // clear and remove cut lines
    for(const auto& l : m_CutLines)
    {
        m_Scene->removeLine(l);
        delete l;
    }

    m_CutLines.clear();

    // clear and remove cut meshes
    for(const auto& m : m_CutMeshes)
    {
        m_Scene->removeMesh(m);
        delete m;
    }

    m_CutMeshes.clear();

    bool can_cut = true;

    switch(m_CutMode)
    {
    case CMM_LINE:
        {
            can_cut = m_Line->isVisible();

            break;
        }
    case CMM_QBEZIER:
        {
            can_cut = m_QBezier->isVisible();

            break;
        }
    case CMM_CBEZIER:
        {
            can_cut = m_CBezier->isVisible();

            break;
        }
    case CMM_SPLINE:
        {
            can_cut = m_Spline->isVisible();

            break;
        }
    default:
        break;
    }

    if(can_cut)
    {
        NSpline<float, 4> rgbg;

        rgbg.addPoint(Vec4f(0.0f, 0.0f, 1.0f, 0.333f));
        rgbg.addPoint(Vec4f(1.0f, 0.0f, 0.0f, 0.333f));
        rgbg.setClosed(false);

        // for each mesh cut
        for(const auto& m : m_Meshes)
        {
            HESMeshVector result;

            bool cutted = false;

            switch(m_CutMode)
            {
            case CMM_LINE:
                {
                    cutted = m_MeshCutter->cutLine(m, m_Line, result);

                    break;
                }
            case CMM_QBEZIER:
                {
                    cutted = m_MeshCutter->cutQuadraticBezier(m, m_QBezier, result);

                    break;
                }
            case CMM_CBEZIER:
                {
                    cutted = m_MeshCutter->cutCubicBezier(m, m_CBezier, result);

                    break;
                }
            case CMM_SPLINE:
                {
                    cutted = m_MeshCutter->cutSpline(m, m_Spline, result);

                    break;
                }
            default:
                break;
            }

            if(!cutted)
                continue;

            m_CutMeshes.insert(m_CutMeshes.end(), result.begin(), result.end());

            // add cut points
            CutPointVector cp_vec;

            for(const auto& cv : m_MeshCutter->getPointCuts())
                for(const auto& cp : cv)
                    cp_vec.push_back(cp);

            const size_t& num_cp = cp_vec.size();

            float s = (num_cp < 2) ? (1.0f) : (1.0f / (float)(num_cp - 1));
            float t = 0.0f;

            rgbg.setPoint(0, Vec4f(0.0f, 0.0f, 1.0f, 0.333f));
            rgbg.setPoint(1, Vec4f(1.0f, 0.0f, 0.0f, 0.333f));

            for(size_t i = 0; i < num_cp; i++)
            {
                Point* cp = new Point();
                cp->setPosition(cp_vec[i]);

                Vec4f c = rgbg.pointAt(t);

                c(0) = std::max(0.0f, std::min(c(0), 1.0f));
                c(1) = std::max(0.0f, std::min(c(1), 1.0f));
                c(2) = std::max(0.0f, std::min(c(2), 1.0f));
                c(3) = std::max(0.0f, std::min(c(3), 1.0f));

                cp->setColour(c);
                cp->setSize(0.02f);

                m_CutPoints.push_back(cp);

                t += s;
            }

            // add cut lines
            const PointCutsVector& cps_vec = m_MeshCutter->getPointCuts();
            const size_t& num_cps = cps_vec.size();

            s = (num_cps < 2) ? (1.0f) : (1.0f / (float)(num_cps - 1));
            t = 0.0f;

            rgbg.setPoint(0, Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
            rgbg.setPoint(1, Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

            for(size_t i = 0; i < num_cps; i++)
            {
                Vec4f c = rgbg.pointAt(t);

                c(0) = std::max(0.0f, std::min(c(0), 1.0f));
                c(1) = std::max(0.0f, std::min(c(1), 1.0f));
                c(2) = std::max(0.0f, std::min(c(2), 1.0f));
                c(3) = std::max(0.0f, std::min(c(3), 1.0f));

                size_t num_cp = cps_vec[i].size();

                for(size_t j = 0; j < num_cp - 1; j++)
                {
                    Line* cl = new Line();
                    cl->setPoint(0, cps_vec[i][j]);
                    cl->setPoint(1, cps_vec[i][j + 1]);
                    cl->setColour(c);

                    m_CutLines.push_back(cl);
                }

                t += s;
            }
        }
    }

    // add cut points to scene
    for(const auto& p : m_CutPoints)
        m_Scene->addPoint(p);

    // add cut lines to scene
    for(const auto& l : m_CutLines)
        m_Scene->addLine(l);

    bool show_source_meshes = m_CutMeshes.empty();

    for(const auto& sm : m_Meshes)
        sm->setVisible(show_source_meshes);

    // add cut meshes to scene
    for(const auto& m : m_CutMeshes)
        m_Scene->addMesh(m);

    m_Renderer->render();
}

void ControllerCutMesh::onCutPressed()
{
    cut();

    m_Renderer->render();
}
