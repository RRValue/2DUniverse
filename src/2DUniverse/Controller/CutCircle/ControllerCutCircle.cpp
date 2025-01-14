#include "2DUniverse/Controller/CutCircle/ControllerCutCircle.h"

#include "2DUniverse/Scene/Scene.h"

#include "2DUniverse/Rendering/Renderer.h"

#include "2DUniverse/Renderables/Point.h"
#include "2DUniverse/Renderables/Line.h"
#include "2DUniverse/Renderables/Circle.h"

#include <QtGui/QMouseEvent>

ControllerCutCircle::ControllerCutCircle() : m_RadusMin(0.001f), m_RadusMax(1.0f)
{
    m_Name = "ControllerCutCircle";
}

ControllerCutCircle::~ControllerCutCircle()
{
    for(const auto& p : m_Points)
        delete p;

    delete m_Line;
    delete m_Circle;

    delete m_CutPoint0;
    delete m_CutPoint1;
}

void ControllerCutCircle::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    m_Line = new Line();
    m_Circle = new Circle();
    m_CutPoint0 = new Point();
    m_CutPoint1 = new Point();

    m_Line->setVisible(false);
    m_Circle->setVisible(false);
    m_CutPoint0->setVisible(false);
    m_CutPoint1->setVisible(false);

    m_Line->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));
    m_Circle->setColour(Vec4f(0.33f, 0.33f, 0.33f, 1.0f));
    m_CutPoint0->setColour(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));
    m_CutPoint1->setColour(Vec4f(1.0f, 0.0f, 0.0f, 0.5f));

    m_CutPoint0->setSize(0.02f);
    m_CutPoint1->setSize(0.02f);

    // create option widget
    m_OptionWidget = new QWidget();
    m_OptionWidgetSetUp.setupUi(m_OptionWidget);

    m_OptionWidgetSetUp.m_RadiusLbl->setText(tr("Radius: %0 ... %1").arg(QString::number(m_RadusMin, 'f', 3)).arg(QString::number(m_RadusMax, 'f', 3)));

    m_RadiusSlider = m_OptionWidgetSetUp.m_RadiusSld;

    setSliderRadius(m_Circle->getRadius());

    connect(m_RadiusSlider, &QSlider::valueChanged, this, &ControllerCutCircle::onRadiusSliderValueChanged);

    // add to scene
    m_Scene->addLine(m_Line);
    m_Scene->addCircle(m_Circle);
    m_Scene->addPoint(m_CutPoint0);
    m_Scene->addPoint(m_CutPoint1);
}

void ControllerCutCircle::activate()
{

}

void ControllerCutCircle::deactivate()
{

}

bool ControllerCutCircle::handleMouseMoveEvent(QMouseEvent* const event)
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

    if(m_CurrentPointIdx == 0)
        m_Line->setPoint(0, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 1)
        m_Line->setPoint(1, m_CurrentPoint->getPosition());

    if(m_CurrentPointIdx == 2)
        m_Circle->setPosition(m_CurrentPoint->getPosition());

    cut();

    m_Renderer->render();

    return true;
}

bool ControllerCutCircle::handleMousePressEvent(QMouseEvent* const event)
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

    // if hit nothing and point size < 3 -> add
    if(m_CurrentPoint == nullptr)
    {
        if(m_Points.size() >= 3)
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

        if(m_CurrentPointIdx == 2)
        {
            m_Circle->setPosition(m_CurrentPoint->getPosition());
            m_Circle->setVisible(true);
        }
    }

    cut();

    m_Renderer->render();

    m_MovePoint = true;
    m_CurrentHitDistance = trans(m_CurrentPoint->getPosition()) - p_f;

    return true;
}

bool ControllerCutCircle::handleMouseReleaseEvent(QMouseEvent* const event)
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

bool ControllerCutCircle::handleResizeEvent(QResizeEvent* const event)
{
    return false;
}

bool ControllerCutCircle::handleWheelEvent(QWheelEvent* const event)
{
    return false;
}

Point* const ControllerCutCircle::getPointAtPos(const Vec2d& pos, size_t* const idx) const
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

void ControllerCutCircle::cut()
{
    if(m_Points.size() != 3)
        return;

    // reset cut point
    m_CutPoint0->setVisible(false);
    m_CutPoint1->setVisible(false);

    IntersectionVector intersections = m_Line->intersect(m_Circle);

    if(intersections.empty())
        return;

    if(intersections.size() >= 1)
    {
        m_CutPoint0->setVisible(true);
        m_CutPoint0->setPosition(intersections[0].m_Point);
    }

    if(intersections.size() >= 2)
    {
        m_CutPoint1->setVisible(true);
        m_CutPoint1->setPosition(intersections[1].m_Point);
    }
}

void ControllerCutCircle::onRadiusSliderValueChanged(int value)
{
    // set to circle
    m_Circle->setRadius(sliderValueToRadius(value));

    // cut
    cut();

    // render
    m_Renderer->render();
}

void ControllerCutCircle::setSliderRadius(const float& radius)
{
    m_RadiusSlider->blockSignals(true);
    m_RadiusSlider->setValue(radiusToSliderValue(radius));
    m_RadiusSlider->blockSignals(false);
}

float ControllerCutCircle::sliderValueToRadius(const int& value)
{
    // slider properties
    float sld_min = (float)m_RadiusSlider->minimum();
    float sld_max = (float)m_RadiusSlider->maximum();
    float range = sld_max - sld_min;

    // radius
    float r = (float)value;

    // to 0 .. 1 range
    r -= sld_min;
    r /= range;

    // to m_RadusMin ... m_RadusMax range
    r *= m_RadusMax - m_RadusMin;
    r += m_RadusMin;

    return r;
}

int ControllerCutCircle::radiusToSliderValue(const float& radius)
{
    // slider properties
    float sld_min = (float)m_RadiusSlider->minimum();
    float sld_max = (float)m_RadiusSlider->maximum();
    float range = sld_max - sld_min;

    // radius
    float v = (float)radius;

    // to 0 .. 1 range
    v -= m_RadusMin;
    v /= m_RadusMax - m_RadusMin;

    // to sld_min ... sld_max range
    v *= range;
    v += sld_min;

    return v;
}
