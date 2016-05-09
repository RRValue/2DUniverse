#include "HalfEdge2D/Controller/CutSpline/ControllerCutSpline.h"

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

ControllerCutSpline::ControllerCutSpline() :
    m_CMin(-1.0f), m_CMax(1.0f),
    m_TMin(-1.0f), m_TMax(1.0f),
    m_BMin(-1.0f), m_BMax(1.0f)
{
    m_Name = "ControllerCutSpline";
}

ControllerCutSpline::~ControllerCutSpline()
{
    for(const auto& p : m_Points)
        delete p;

    for(const auto& p : m_CutPoints)
        delete p;

    delete m_Spline;
    delete m_Line;
}

void ControllerCutSpline::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Spline = new Spline();
    m_Spline->setColour(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    m_Spline->setVisible(false);

    m_Line = new Line();
    m_Line->setColour(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    m_Line->setVisible(false);

    // create option widget
    m_OptionWidget = new QWidget();
    m_OptionWidgetSetUp.setupUi(m_OptionWidget);

    m_OptionWidgetSetUp.m_CLbl->setText(tr("C(%0...%1)").arg(QString::number(m_CMin, 'f', 0)).arg(QString::number(m_CMax, 'f', 0)));
    m_OptionWidgetSetUp.m_TLbl->setText(tr("T(%0...%1)").arg(QString::number(m_TMin, 'f', 0)).arg(QString::number(m_TMax, 'f', 0)));
    m_OptionWidgetSetUp.m_BLbl->setText(tr("B(%0...%1)").arg(QString::number(m_BMin, 'f', 0)).arg(QString::number(m_BMax, 'f', 0)));

    m_CSlider = m_OptionWidgetSetUp.m_CSld;
    m_TSlider = m_OptionWidgetSetUp.m_TSld;
    m_BSlider = m_OptionWidgetSetUp.m_BSld;
    m_ClosedCkb = m_OptionWidgetSetUp.m_CloseCkb;

    setSliderC(0.0f);
    setSliderT(0.0f);
    setSliderB(0.0f);

    connect(m_CSlider, &QSlider::valueChanged, this, &ControllerCutSpline::onSliderValueChanged);
    connect(m_TSlider, &QSlider::valueChanged, this, &ControllerCutSpline::onSliderValueChanged);
    connect(m_BSlider, &QSlider::valueChanged, this, &ControllerCutSpline::onSliderValueChanged);
    connect(m_ClosedCkb, &QCheckBox::stateChanged, this, &ControllerCutSpline::onClosedChanged);

    // add to scene
    m_Scene->addSpline(m_Spline);
    m_Scene->addLine(m_Line);
}

void ControllerCutSpline::activate()
{

}

void ControllerCutSpline::deactivate()
{

}

bool ControllerCutSpline::handleMouseMoveEvent(QMouseEvent* const event)
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
        m_Line->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());
    else if(m_CurrentPointIdx == 1)
    {
        m_Line->setVisible(true);
        m_Line->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());
    }
    else
    {
        m_Spline->setVisible(true);
        m_Spline->setPoint(m_CurrentPointIdx - 2, m_CurrentPoint->getPosition());
    }

    cut();

    m_Renderer->render();

    return true;
}

bool ControllerCutSpline::handleMousePressEvent(QMouseEvent* const event)
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

    // if hit nothing and point size < 12 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 12)
        {
            m_MovePoint = false;

            return true;
        }

        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));
        m_CurrentPoint->setSize(0.025f);
        m_CurrentPoint->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

        m_CurrentPointIdx = m_Points.size();

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_CurrentPointIdx == 0)
            m_Line->setPoint(0, m_CurrentPoint->getPosition());
        else if(m_CurrentPointIdx == 1)
        {
            m_Line->setVisible(true);
            m_Line->setPoint(1, m_CurrentPoint->getPosition());
        }
        else
        {
            if(m_CurrentPointIdx > 2)
                m_Spline->setVisible(true);

            m_Spline->addPoint(m_CurrentPoint->getPosition());
        }
    }

    cut();

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCutSpline::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerCutSpline::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutSpline::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCutSpline::getPointAtPos(const Vec2f& pos, size_t* const idx) const
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

void ControllerCutSpline::onSliderValueChanged(int value)
{
    QObject* sendet_from = sender();

    if(sendet_from == m_CSlider)
        m_Spline->setContinuity(sliderValueToValue(value, m_CSlider->minimum(), m_CSlider->maximum(), m_CMin, m_CMax));
    else if(sendet_from == m_TSlider)
        m_Spline->setTension(sliderValueToValue(value, m_TSlider->minimum(), m_TSlider->maximum(), m_TMin, m_TMax));
    else if(sendet_from == m_BSlider)
        m_Spline->setBias(sliderValueToValue(value, m_BSlider->minimum(), m_BSlider->maximum(), m_BMin, m_BMax));

    cut();

    m_Renderer->render();
}

void ControllerCutSpline::onClosedChanged(int state)
{
    QObject* sendet_from = sender();

    if(sendet_from == nullptr)
        return;

    if(state != Qt::Checked && state != Qt::Unchecked)
        return;

    bool close = state == Qt::Checked;

    if(m_Spline->isClosed() == close)
        return;

    m_Spline->setClosed(close);

    cut();

    m_Renderer->render();
}

void ControllerCutSpline::setSliderC(const float& c)
{
    m_CSlider->blockSignals(true);
    m_CSlider->setValue(valueToSliderValue(c, m_CSlider->minimum(), m_CSlider->maximum(), m_CMin, m_CMax));
    m_CSlider->blockSignals(false);
}

void ControllerCutSpline::setSliderT(const float& t)
{
    m_TSlider->blockSignals(true);
    m_TSlider->setValue(valueToSliderValue(t, m_TSlider->minimum(), m_TSlider->maximum(), m_TMin, m_TMax));
    m_TSlider->blockSignals(false);
}

void ControllerCutSpline::setSliderB(const float& b)
{
    m_BSlider->blockSignals(true);
    m_BSlider->setValue(valueToSliderValue(b, m_BSlider->minimum(), m_BSlider->maximum(), m_BMin, m_BMax));
    m_BSlider->blockSignals(false);
}

float ControllerCutSpline::sliderValueToValue(const int& value, const int& sliderMin, const int& sliderMax, const float& min, const float& max)
{
    // slider properties
    float sld_min = (float)sliderMin;
    float sld_max = (float)sliderMax;
    float range = sld_max - sld_min;

    // radius
    float r = (float)value;

    // to 0 .. 1 range
    r -= sld_min;
    r /= range;

    // to m_RadusMin ... m_RadusMax range
    r *= max - min;
    r += min;

    return r;
}

float ControllerCutSpline::valueToSliderValue(const float& value, const int& sliderMin, const int& sliderMax, const float& min, const float& max)
{
    // slider properties
    float sld_min = (float)sliderMin;
    float sld_max = (float)sliderMax;
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

void ControllerCutSpline::cut()
{
    if(!m_Line->isVisible() || !m_Spline->isVisible())
        return;

    Vec2fVector cut_points = m_Line->intersect(*m_Spline);

    // delete old one
    for(const auto& p : m_CutPoints)
    {
        m_Scene->removePoint(p);
        delete p;
    }

    m_CutPoints.clear();

    // add new ones
    for(const auto& cp : cut_points)
    {
        Point* np = new Point();
        np->setVisible(true);
        np->setPosition(cp);
        np->setSize(0.025f);
        np->setColour(Vec4f(0.0f, 0.0f, 1.0f, 0.5f));

        m_Scene->addPoint(np);
        m_CutPoints.push_back(np);
    }
}
