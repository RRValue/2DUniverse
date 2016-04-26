#include "HalfEdge2D/Controller/SplitAt/ControllerSplitAt.h"

#include "HalfEdge2D/Rendering/RenderTarget.h"
#include "HalfEdge2D/Rendering/Renderer.h"

#include "HalfEdge2D/Scene/Scene.h"
#include "HalfEdge2D/Scene/ViewPort.h"
#include "HalfEdge2D/Scene/Camera.h"

#include "HalfEdge2D/Renderables/Point.h"
#include "HalfEdge2D/Renderables/Line.h"
#include "HalfEdge2D/Renderables/Spline.h"

#include "HalfEdge2D/HalfEdge/HESMesh.h"

#include "HalfEdge2D/Mesh/Vertex.h"

#include <QtGui/QMouseEvent>

ControllerSplitAt::ControllerSplitAt() :
    m_SplitMin(0.0f), m_SplitMax(1.0f)
{
    m_Name = "ControllerSplitAt";
}

ControllerSplitAt::~ControllerSplitAt()
{
    for(const auto& p : m_LinePoints)
        delete p;

    for(const auto& p : m_SplitLinePoints)
        delete p;

    for(const auto& p : m_QBezierPoints)
        delete p;

    for(const auto& p : m_SplitQBezierPoints)
        delete p;

    for(const auto& p : m_CBezierPoints)
        delete p;

    for(const auto& p : m_SplitCBezierPoints)
        delete p;

    delete m_Line;
    delete m_Line;

    delete m_QBezier;
    delete m_SplitQBezier;

    delete m_CBezier;
    delete m_SplitCBezier;
}

void ControllerSplitAt::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Line = new Line();
    m_Line->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_Line->setVisible(false);

    m_SplitLine = new Line();
    m_SplitLine->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_SplitLine->setVisible(false);

    m_QBezier = new QuadraticBezier();
    m_QBezier->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_QBezier->setVisible(false);

    m_SplitQBezier = new QuadraticBezier();
    m_SplitQBezier->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_SplitQBezier->setVisible(false);

    m_CBezier = new CubicBezier();
    m_CBezier->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_CBezier->setVisible(false);

    m_SplitCBezier = new CubicBezier();
    m_SplitCBezier->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_SplitCBezier->setVisible(false);

    // create option widget
    m_OptionWidget = new QWidget();
    m_OptionWidgetSetUp.setupUi(m_OptionWidget);

    m_OptionWidgetSetUp.m_SplitAtLbl->setText(tr("S(%0...%1)").arg(QString::number(m_SplitMin, 'f', 0)).arg(QString::number(m_SplitMax, 'f', 0)));

    m_SplitSlider = m_OptionWidgetSetUp.m_SplitAtSld;
    m_TypeCbb = m_OptionWidgetSetUp.m_TypeCbb;

    setSliderSplit(0.0f);

    m_TypeCbb->addItem("Line", (int)E_ST_LINE);
    m_TypeCbb->addItem("QuadraticBezier", (int)E_ST_QBEZIER);
    m_TypeCbb->addItem("CubicBezier", (int)E_ST_CBEZIER);

    onTypeChanged(0);
    
    connect(m_SplitSlider, &QSlider::sliderMoved, this, &ControllerSplitAt::onSliderMoved);
    connect(m_TypeCbb, SIGNAL(activated(int)), this, SLOT(onTypeChanged(int)));

    // add to scene
    m_Scene->addLine(m_Line);
    m_Scene->addLine(m_SplitLine);
    m_Scene->addQuadraticBeziers(m_QBezier);
    m_Scene->addQuadraticBeziers(m_SplitQBezier);
    m_Scene->addCubicBeziers(m_CBezier);
    m_Scene->addCubicBeziers(m_SplitCBezier);
}

void ControllerSplitAt::activate()
{

}

void ControllerSplitAt::deactivate()
{

}

bool ControllerSplitAt::handleMouseMoveEvent(QMouseEvent* const event)
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

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            m_LinePoints[m_CurrentPointIdx]->setPosition(new_pos);
            m_SplitLinePoints[m_CurrentPointIdx]->setPosition(new_pos);

            m_Line->setPoint(m_CurrentPointIdx, new_pos);
            m_SplitLine->setPoint(m_CurrentPointIdx, new_pos);

            break;
        }
    case E_ST_QBEZIER:
        {
            m_QBezierPoints[m_CurrentPointIdx]->setPosition(new_pos);
            m_SplitQBezierPoints[m_CurrentPointIdx]->setPosition(new_pos);

            m_QBezier->setPoint(m_CurrentPointIdx, new_pos);
            m_SplitQBezier->setPoint(m_CurrentPointIdx, new_pos);

            break;
        }
    case E_ST_CBEZIER:
        {
            m_CBezierPoints[m_CurrentPointIdx]->setPosition(new_pos);
            m_SplitCBezierPoints[m_CurrentPointIdx]->setPosition(new_pos);

            m_CBezier->setPoint(m_CurrentPointIdx, new_pos);
            m_SplitCBezier->setPoint(m_CurrentPointIdx, new_pos);

            break;
        }
    default:
        break;
    }

    updateSplit();

    m_Renderer->render();

    return true;
}

bool ControllerSplitAt::handleMousePressEvent(QMouseEvent* const event)
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

    Vec2f pos = invTrans(p_f);

    m_CurrentPoint = getPointAtPos(pos, &m_CurrentPointIdx);

    if(m_CurrentPoint == nullptr)
        if(addPoint(pos))
        {
            updateSplit();
        }
        else
            return true;

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

void ControllerSplitAt::updateVisibility()
{
    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            setLineVisible(true);
            setQBezVisible(false);
            setCBezVisible(false);

            break;
        }
    case E_ST_QBEZIER:
        {
            setLineVisible(false);
            setQBezVisible(true);
            setCBezVisible(false);
            break;
        }
    case E_ST_CBEZIER:
        {
            setLineVisible(false);
            setQBezVisible(false);
            setCBezVisible(true);

            break;
        }
    default:
        break;
    }
}

void ControllerSplitAt::setLineVisible(const bool& visible)
{
    bool l_visible = m_CurrentNumPoints == m_CurrentNumMaxPoints;

    for(const auto& p : m_LinePoints)
        p->setVisible(visible);

    for(const auto& p : m_SplitLinePoints)
        p->setVisible(m_Splitable && visible);

    m_Line->setVisible(visible && l_visible);
    m_SplitLine->setVisible(m_Splitable && visible && l_visible);
}

void ControllerSplitAt::setQBezVisible(const bool& visible)
{
    bool l_visible = m_CurrentNumPoints == m_CurrentNumMaxPoints;

    for(const auto& p : m_QBezierPoints)
        p->setVisible(visible);

    for(const auto& p : m_SplitQBezierPoints)
        p->setVisible(m_Splitable && visible);

    m_QBezier->setVisible(visible && l_visible);
    m_SplitQBezier->setVisible(m_Splitable && visible && l_visible);
}

void ControllerSplitAt::setCBezVisible(const bool& visible)
{
    bool l_visible = m_CurrentNumPoints == m_CurrentNumMaxPoints;

    for(const auto& p : m_CBezierPoints)
        p->setVisible(visible);

    for(const auto& p : m_SplitCBezierPoints)
        p->setVisible(m_Splitable && visible);

    m_CBezier->setVisible(visible && l_visible);
    m_SplitCBezier->setVisible(m_Splitable && visible && l_visible);
}

bool ControllerSplitAt::addPoint(const Vec2f& pos)
{
    if(m_CurrentNumPoints >= m_CurrentNumMaxPoints)
    {
        m_MovePoint = false;

        return false;
    }

    Point* new_point0 = new Point();
    Point* new_point1 = new Point();

    m_CurrentPoint = new_point0;
    
    new_point0->setPosition(pos);
    new_point1->setPosition(pos);

    m_Scene->addPoint(new_point0);
    m_Scene->addPoint(new_point1);

    m_CurrentPointIdx = m_CurrentNumPoints;

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            m_LinePoints.push_back(new_point0);
            m_SplitLinePoints.push_back(new_point1);
    
            m_Line->setPoint(m_CurrentPointIdx, pos);
            m_SplitLine->setPoint(m_CurrentPointIdx, pos);

            m_CurrentNumPoints++;

            break;
        }
    case E_ST_QBEZIER:
        {
            m_QBezierPoints.push_back(new_point0);
            m_SplitQBezierPoints.push_back(new_point1);

            m_QBezier->setPoint(m_CurrentPointIdx, pos);
            m_SplitQBezier->setPoint(m_CurrentPointIdx, pos);

            m_CurrentNumPoints++;

            break;
        }
    case E_ST_CBEZIER:
        {
            m_CBezierPoints.push_back(new_point0);
            m_SplitCBezierPoints.push_back(new_point1);

            m_CBezier->setPoint(m_CurrentPointIdx, pos);
            m_SplitCBezier->setPoint(m_CurrentPointIdx, pos);

            m_CurrentNumPoints++;

            break;
        }
    default:
        break;
    }
}

bool ControllerSplitAt::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerSplitAt::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerSplitAt::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerSplitAt::getPointAtPos(const Vec2f& pos, size_t* const idx) const
{
    std::vector<Point* const> c_points;

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            c_points = m_LinePoints;
            break;
        }
    case E_ST_QBEZIER:
        {
            c_points = m_QBezierPoints;
            break;
        }
    case E_ST_CBEZIER:
        {
            c_points = m_CBezierPoints;
            break;
        }
    }

    if(c_points.empty())
        return nullptr;

    for(size_t i = 0; i < c_points.size(); i++)
    {
        if((c_points[i]->getPosition() - pos).norm() >= c_points[i]->getSize())
            continue;

        *idx = i;

        return c_points[i];
    }

    return nullptr;
}

void ControllerSplitAt::updateSplit()
{
    float s = sliderValueToValue(m_SplitSlider, m_SplitMin, m_SplitMax);

    m_Splitable = s > 0.0 && s < 1.0;

    updateVisibility();
}

void ControllerSplitAt::onSliderMoved(int value)
{
    QObject* sendet_from = sender();

    if(sendet_from == nullptr || sendet_from != m_SplitSlider)
        return;
    
    updateSplit();

    m_Renderer->render();
}

void ControllerSplitAt::onTypeChanged(int idx)
{
    m_SplitType = (E_SplitType)m_TypeCbb->itemData(idx).toInt();

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            m_CurrentNumMaxPoints = 2;
            m_CurrentNumPoints = m_LinePoints.size();
            
            break;
        }
    case E_ST_QBEZIER:
        {
            m_CurrentNumMaxPoints = 3;
            m_CurrentNumPoints = m_QBezierPoints.size();

            break;
        }
    case E_ST_CBEZIER:
        {
            m_CurrentNumMaxPoints = 4;
            m_CurrentNumPoints = m_CBezierPoints.size();

            break;
        }
    default:
        {
            m_CurrentNumMaxPoints = 0;
            m_CurrentNumPoints = 0;

            break;
        }
    }

    updateSplit();

    m_Renderer->render();
}

void ControllerSplitAt::setSliderSplit(const float& s)
{
    m_SplitSlider->blockSignals(true);
    m_SplitSlider->setValue(valueToSliderValue(s, m_SplitSlider, m_SplitMin, m_SplitMax));
    m_SplitSlider->blockSignals(false);
}

float ControllerSplitAt::sliderValueToValue(QSlider* const sld, const float& min, const float& max)
{
    // slider properties
    float sld_min = (float)sld->minimum();
    float sld_max = (float)sld->maximum();
    float range = sld_max - sld_min;

    // radius
    float r = (float)sld->value();

    // to 0 .. 1 range
    r -= sld_min;
    r /= range;

    // to m_RadusMin ... m_RadusMax range
    r *= max - min;
    r += min;

    return r;
}

int ControllerSplitAt::valueToSliderValue(const float& value, QSlider* const sld, const float& min, const float& max)
{
    // slider properties
    float sld_min = (float)sld->minimum();
    float sld_max = (float)sld->maximum();
    float range = sld_max - sld_min;

    // radius
    float v = (float)value;

    // to 0 .. 1 range
    v -= min;
    v /= max - min;

    // to sld_min ... sld_max range
    v *= range;
    v += sld_min;

    return v;
}
