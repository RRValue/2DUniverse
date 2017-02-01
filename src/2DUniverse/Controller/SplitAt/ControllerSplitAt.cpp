#include "2DUniverse/Controller/SplitAt/ControllerSplitAt.h"

#include "2DUniverse/Rendering/Renderer.h"

#include "2DUniverse/Scene/Scene.h"

#include "2DUniverse/Renderables/Point.h"
#include "2DUniverse/Renderables/Line.h"
#include "2DUniverse/Renderables/QuadraticBezier.h"
#include "2DUniverse/Renderables/CubicBezier.h"

#include <QtGui/QMouseEvent>

ControllerSplitAt::ControllerSplitAt() :
    m_SplitMin(0.0f), m_SplitMax(1.0f),
    m_ColourOrg(Vec4f(0.0f, 0.0f, 0.0f, 1.0f)),
    m_ColourL(Vec4f(1.0f, 0.0f, 0.0f, 0.5f)),
    m_ColourR(Vec4f(0.0f, 0.0f, 1.0f, 0.5f))
{
    m_Name = "ControllerSplitAt";
}

ControllerSplitAt::~ControllerSplitAt()
{
    for(size_t i = 0; i < 3; i++)
    {
        for(const auto& p : m_LinePoints[i])
            delete p;

        for(const auto& p : m_QBezierPoints[i])
            delete p;

        for(const auto& p : m_CBezierPoints[i])
            delete p;

        delete m_Line[i];
        delete m_QBezier[i];
        delete m_CBezier[i];
    }
}

void ControllerSplitAt::init()
{
    m_MovePoint = false;
    m_CurrentPoint = nullptr;

    for(size_t i = 0; i < 3; i++)
    {
        Vec4f c;

        if(i == 0)
            c = m_ColourOrg;
        else if(i == 1)
            c = m_ColourL;
        else if(i == 2)
            c = m_ColourR;

        m_Line[i] = new Line();
        m_Line[i]->setColour(c);
        m_Line[i]->setVisible(false);

        m_QBezier[i] = new QuadraticBezier();
        m_QBezier[i]->setColour(c);
        m_QBezier[i]->setVisible(false);

        m_CBezier[i] = new CubicBezier();
        m_CBezier[i]->setColour(c);
        m_CBezier[i]->setVisible(false);
    }

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
    
    connect(m_SplitSlider, &QSlider::valueChanged, this, &ControllerSplitAt::onSliderValueChanged);
    connect(m_TypeCbb, SIGNAL(activated(int)), this, SLOT(onTypeChanged(int)));

    // add to scene
    for(size_t i = 0; i < 3; i++)
    {
        m_Scene->addLine(m_Line[i]);
        m_Scene->addQuadraticBeziers(m_QBezier[i]);
        m_Scene->addCubicBeziers(m_CBezier[i]);
    }
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
    Vec2d pos = Vec2d((float)pos_in_vp[0], (float)pos_in_vp[1]) + m_CurrentHitDistance;
    Vec2d new_pos = invTrans(pos);

    m_CurrentPoint->setPosition(new_pos);

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            for(size_t i = 0; i < 3; i++)
            {
                m_LinePoints[i][m_CurrentPointIdx]->setPosition(new_pos);
                m_Line[i]->setPoint(m_CurrentPointIdx, new_pos);
            }

            break;
        }
    case E_ST_QBEZIER:
        {
            for(size_t i = 0; i < 3; i++)
            {
                m_QBezierPoints[i][m_CurrentPointIdx]->setPosition(new_pos);
                m_QBezier[i]->setPoint(m_CurrentPointIdx, new_pos);
            }

            break;
        }
    case E_ST_CBEZIER:
        {
            for(size_t i = 0; i < 3; i++)
            {
                m_CBezierPoints[i][m_CurrentPointIdx]->setPosition(new_pos);
                m_CBezier[i]->setPoint(m_CurrentPointIdx, new_pos);
            }

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
    Vec2d p_f(p[0], p[1]);

    Vec2d pos = invTrans(p_f);

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

    for(const auto& p : m_LinePoints[0])
        p->setVisible(visible);

    m_Line[0]->setVisible(visible && l_visible);

    for(size_t i = 1; i < 3; i++)
    {
        for(const auto& p : m_LinePoints[i])
            p->setVisible(m_Splitable && visible);

        m_Line[i]->setVisible(m_Splitable && visible && l_visible);
    }
}

void ControllerSplitAt::setQBezVisible(const bool& visible)
{
    bool l_visible = m_CurrentNumPoints == m_CurrentNumMaxPoints;

    for(const auto& p : m_QBezierPoints[0])
        p->setVisible(visible);

    m_QBezier[0]->setVisible(visible && l_visible);

    for(size_t i = 1; i < 3; i++)
    {
        for(const auto& p : m_QBezierPoints[i])
            p->setVisible(m_Splitable && visible);

        m_QBezier[i]->setVisible(m_Splitable && visible && l_visible);
    }
}

void ControllerSplitAt::setCBezVisible(const bool& visible)
{
    bool l_visible = m_CurrentNumPoints == m_CurrentNumMaxPoints;

    for(const auto& p : m_CBezierPoints[0])
        p->setVisible(visible);

    m_CBezier[0]->setVisible(visible && l_visible);

    for(size_t i = 1; i < 3; i++)
    {
        for(const auto& p : m_CBezierPoints[i])
            p->setVisible(m_Splitable && visible);

        m_CBezier[i]->setVisible(m_Splitable && visible && l_visible);
    }
}

bool ControllerSplitAt::addPoint(const Vec2d& pos)
{
    if(m_CurrentNumPoints >= m_CurrentNumMaxPoints)
    {
        m_MovePoint = false;

        return false;
    }

    Point* new_points[3];
    
    for(size_t i = 0; i < 3; i++)
    {
        Vec4f c;

        if(i == 0)
            c = m_ColourOrg;
        else if(i == 1)
            c = m_ColourL;
        else if(i == 2)
            c = m_ColourR;

        new_points[i] = new Point();
        new_points[i]->setPosition(pos);
        new_points[i]->setColour(c);

        m_Scene->addPoint(new_points[i]);
    }

    m_CurrentPoint = new_points[0];

    m_CurrentPointIdx = m_CurrentNumPoints;

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            for(size_t i = 0; i < 3; i++)
            {
                m_LinePoints[i].push_back(new_points[i]);
                m_Line[i]->setPoint(m_CurrentPointIdx, pos);
            }
            
            m_CurrentNumPoints++;

            break;
        }
    case E_ST_QBEZIER:
        {
            for(size_t i = 0; i < 3; i++)
            {
                m_QBezierPoints[i].push_back(new_points[i]);
                m_QBezier[i]->setPoint(m_CurrentPointIdx, pos);
            }

            m_CurrentNumPoints++;

            break;
        }
    case E_ST_CBEZIER:
        {
            for(size_t i = 0; i < 3; i++)
            {
                m_CBezierPoints[i].push_back(new_points[i]);
                m_CBezier[i]->setPoint(m_CurrentPointIdx, pos);
            }

            m_CurrentNumPoints++;

            break;
        }
    default:
        break;
    }

    return true;
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

Point* const ControllerSplitAt::getPointAtPos(const Vec2d& pos, size_t* const idx) const
{
    std::vector<Point*> c_points;

    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            c_points = m_LinePoints[0];
            break;
        }
    case E_ST_QBEZIER:
        {
            c_points = m_QBezierPoints[0];
            break;
        }
    case E_ST_CBEZIER:
        {
            c_points = m_CBezierPoints[0];
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

    // split
    switch(m_SplitType)
    {
    case E_ST_LINE:
        {
            if(!m_Line[0]->isVisible())
                break;

            m_Line[0]->splitAt(s, *m_Line[1], *m_Line[2]);

            Vec2d d(0.0, 0.1);

            if(!m_Line[1]->isVisible() || !m_Line[2]->isVisible())
                break;

            for(size_t i = 1; i < 3; i++)
            {
                for(size_t j = 0; j < m_CurrentNumMaxPoints; j++)
                {
                    m_Line[i]->setPoint(j, m_Line[i]->getPoint(j) + d);

                    m_LinePoints[i][j]->setPosition(m_Line[i]->getPoint(j));
                }
            }

            break;
        }
    case E_ST_QBEZIER:
        {
            if(!m_QBezier[0]->isVisible())
                break;;

            m_QBezier[0]->getSection(*m_QBezier[1], 0.0, s);
            m_QBezier[0]->getSection(*m_QBezier[2], s, 1.0);

            Vec2d d(0.0, 0.1);

            if(!m_QBezier[1]->isVisible() || !m_QBezier[2]->isVisible())
                break;

            for(size_t i = 1; i < 3; i++)
            {
                for(size_t j = 0; j < m_CurrentNumMaxPoints; j++)
                {
                    m_QBezier[i]->setPoint(j, m_QBezier[i]->getPoint(j) + d);

                    m_QBezierPoints[i][j]->setPosition(m_QBezier[i]->getPoint(j));
                }
            }

            break;
        }
    case E_ST_CBEZIER:
        {
            if(!m_CBezier[0]->isVisible())
                break;

            m_CBezier[0]->getSection(*m_CBezier[1], 0.0, s);
            m_CBezier[0]->getSection(*m_CBezier[2], s, 1.0);

            Vec2d d(0.0, 0.1);

            if(!m_CBezier[1]->isVisible() || !m_CBezier[2]->isVisible())
                break;

            for(size_t i = 1; i < 3; i++)
            {
                for(size_t j = 0; j < m_CurrentNumMaxPoints; j++)
                {
                    m_CBezier[i]->setPoint(j, m_CBezier[i]->getPoint(j) + d);

                    m_CBezierPoints[i][j]->setPosition(m_CBezier[i]->getPoint(j));
                }
            }

            break;
        }
    default:
        break;
    }
}

void ControllerSplitAt::onSliderValueChanged(int value)
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
            m_CurrentNumPoints = m_LinePoints[0].size();
            
            break;
        }
    case E_ST_QBEZIER:
        {
            m_CurrentNumMaxPoints = 3;
            m_CurrentNumPoints = m_QBezierPoints[0].size();

            break;
        }
    case E_ST_CBEZIER:
        {
            m_CurrentNumMaxPoints = 4;
            m_CurrentNumPoints = m_CBezierPoints[0].size();

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
