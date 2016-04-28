#include "HalfEdge2D/Controller/SplineAt/ControllerSplineAt.h"

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

ControllerSplineAt::ControllerSplineAt() :
    m_CMin(-1.0f), m_CMax(1.0f),
    m_TMin(-1.0f), m_TMax(1.0f),
    m_BMin(-1.0f), m_BMax(1.0f),
    m_AMin(0.0f), m_AMax(1.0f)
{
    m_Name = "ControllerSplineAt";
}

ControllerSplineAt::~ControllerSplineAt()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Spline;

    delete m_Point;
    delete m_Tangent;
    delete m_Normal;
}

void ControllerSplineAt::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Spline = new Spline();
    m_Spline->setColour(Vec4f(0.0f, 0.0f, 0.0f, 0.2f));
    m_Spline->setVisible(false);

    m_Point = new Point();
    m_Point->setColour(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    m_Point->setVisible(false);

    m_Tangent = new Line();
    m_Tangent->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    m_Tangent->setVisible(false);

    m_Normal = new Line();
    m_Normal->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_Normal->setVisible(false);

    m_PointN = new Point();
    m_PointN->setColour(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    m_PointN->setVisible(false);

    m_TangentN = new Line();
    m_TangentN->setColour(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    m_TangentN->setVisible(false);

    m_NormalN = new Line();
    m_NormalN->setColour(Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    m_NormalN->setVisible(false);

    // create option widget
    m_OptionWidget = new QWidget();
    m_OptionWidgetSetUp.setupUi(m_OptionWidget);

    m_OptionWidgetSetUp.m_CLbl->setText(tr("C(%0...%1)").arg(QString::number(m_CMin, 'f', 0)).arg(QString::number(m_CMax, 'f', 0)));
    m_OptionWidgetSetUp.m_TLbl->setText(tr("T(%0...%1)").arg(QString::number(m_TMin, 'f', 0)).arg(QString::number(m_TMax, 'f', 0)));
    m_OptionWidgetSetUp.m_BLbl->setText(tr("B(%0...%1)").arg(QString::number(m_BMin, 'f', 0)).arg(QString::number(m_BMax, 'f', 0)));
    m_OptionWidgetSetUp.m_AlphaLbl->setText(tr("B(%0...%1)").arg(QString::number(m_AMin, 'f', 0)).arg(QString::number(m_AMax, 'f', 0)));

    m_CSlider = m_OptionWidgetSetUp.m_CSld;
    m_TSlider = m_OptionWidgetSetUp.m_TSld;
    m_BSlider = m_OptionWidgetSetUp.m_BSld;
    m_ASlider = m_OptionWidgetSetUp.m_AlphaSld;
    m_ClosedCkb = m_OptionWidgetSetUp.m_CloseCkb;

    setSliderC(0.0f);
    setSliderT(0.0f);
    setSliderB(0.0f);
    setSliderA(0.0f);

    connect(m_CSlider, &QSlider::sliderMoved, this, &ControllerSplineAt::onSliderMoved);
    connect(m_TSlider, &QSlider::sliderMoved, this, &ControllerSplineAt::onSliderMoved);
    connect(m_BSlider, &QSlider::sliderMoved, this, &ControllerSplineAt::onSliderMoved);
    connect(m_ASlider, &QSlider::sliderMoved, this, &ControllerSplineAt::onSliderMoved);
    connect(m_ClosedCkb, &QCheckBox::stateChanged, this, &ControllerSplineAt::onClosedChanged);

    // add to scene
    m_Scene->addSpline(m_Spline);
    m_Scene->addPoint(m_Point);
    m_Scene->addLine(m_Tangent);
    m_Scene->addLine(m_Normal);
    m_Scene->addPoint(m_PointN);
    m_Scene->addLine(m_TangentN);
    m_Scene->addLine(m_NormalN);
}

void ControllerSplineAt::activate()
{

}

void ControllerSplineAt::deactivate()
{

}

bool ControllerSplineAt::handleMouseMoveEvent(QMouseEvent* const event)
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

    m_Spline->setPoint(m_CurrentPointIdx, m_CurrentPoint->getPosition());

    updateData();

    m_Renderer->render();

    return true;
}

bool ControllerSplineAt::handleMousePressEvent(QMouseEvent* const event)
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

    // if hit nothing and point size < 10 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 10)
        {
            m_MovePoint = false;

            return true;
        }

        m_CurrentPoint = new Point();
        m_CurrentPoint->setPosition(invTrans(p_f));
        
        m_CurrentPointIdx = m_Points.size();

        m_Spline->addPoint(m_CurrentPoint->getPosition());

        m_Points.push_back(m_CurrentPoint);
        m_Scene->addPoint(m_CurrentPoint);

        if(m_Points.size() == 2)
            m_Spline->setVisible(true);

        updateData();
    }

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerSplineAt::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerSplineAt::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerSplineAt::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerSplineAt::getPointAtPos(const Vec2f& pos, size_t* const idx) const
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

void ControllerSplineAt::updateData()
{
    if(!m_Spline->isVisible() || m_Spline->getNumControllPoints() < 2)
        return;

    float alpha = sliderValueToValue(m_ASlider, m_AMin, m_AMax);

    Vec2f p_at0 = m_Spline->pointAt(alpha);
    Vec2f p_at1 = m_Spline->pointAtL(alpha);
    Vec2f t_at0 = m_Spline->tangentAt(alpha) / 10.0f;
    Vec2f t_at1 = m_Spline->tangentAtL(alpha) / 10.0f;
    Vec2f n_at0 = m_Spline->normalAt(alpha) / 10.0f;
    Vec2f n_at1 = m_Spline->normalAtL(alpha) / 10.0f;

    m_Point->setVisible(true);
    m_Point->setPosition(p_at0);

    m_Tangent->setVisible(true);
    m_Tangent->setPoint(0, p_at0);
    m_Tangent->setPoint(1, p_at0 + t_at0);

    m_Normal->setVisible(true);
    m_Normal->setPoint(0, p_at0);
    m_Normal->setPoint(1, p_at0 + n_at0);

    m_PointN->setVisible(true);
    m_PointN->setPosition(p_at1);

    m_TangentN->setVisible(true);
    m_TangentN->setPoint(0, p_at1);
    m_TangentN->setPoint(1, p_at1 + t_at1);

    m_NormalN->setVisible(true);
    m_NormalN->setPoint(0, p_at1);
    m_NormalN->setPoint(1, p_at1 + n_at1);

    m_Renderer->render();
}

void ControllerSplineAt::onSliderMoved(int value)
{
    QObject* sendet_from = sender();

    if(sendet_from == m_CSlider)
        m_Spline->setContinuity(sliderValueToValue(m_CSlider, m_CMin, m_CMax));
    else if(sendet_from == m_TSlider)
        m_Spline->setTension(sliderValueToValue(m_TSlider, m_TMin, m_TMax));
    else if(sendet_from == m_BSlider)
        m_Spline->setBias(sliderValueToValue(m_BSlider, m_BMin, m_BMax));
    
    updateData();

    m_Renderer->render();
}

void ControllerSplineAt::onClosedChanged(int state)
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

    updateData();

    m_Renderer->render();
}

void ControllerSplineAt::setSliderC(const float& c)
{
    m_CSlider->blockSignals(true);
    m_CSlider->setValue(valueToSliderValue(c, m_CSlider, m_CMin, m_CMax));
    m_CSlider->blockSignals(false);
}

void ControllerSplineAt::setSliderT(const float& t)
{
    m_TSlider->blockSignals(true);
    m_TSlider->setValue(valueToSliderValue(t, m_TSlider, m_TMin, m_TMax));
    m_TSlider->blockSignals(false);
}

void ControllerSplineAt::setSliderB(const float& b)
{
    m_BSlider->blockSignals(true);
    m_BSlider->setValue(valueToSliderValue(b, m_BSlider, m_BMin, m_BMax));
    m_BSlider->blockSignals(false);
}

void ControllerSplineAt::setSliderA(const float& a)
{
    m_ASlider->blockSignals(true);
    m_ASlider->setValue(valueToSliderValue(a, m_ASlider, m_AMin, m_AMax));
    m_ASlider->blockSignals(false);
}

float ControllerSplineAt::sliderValueToValue(QSlider* const sld, const float& min, const float& max)
{
    // slider properties
    float sld_min = (float)sld->minimum();
    float sld_max = (float)sld->maximum();
    float range = sld_max - 1 - sld_min;

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

int ControllerSplineAt::valueToSliderValue(const float& value, QSlider* const sld, const float& min, const float& max)
{
    // slider properties
    float sld_min = (float)sld->minimum();
    float sld_max = (float)sld->maximum();
    float range = sld_max - 1 - sld_min;

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
