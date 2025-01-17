#ifndef _CONTROLLER_CONTROLLERSPLITAT_H_
#define _CONTROLLER_CONTROLLERSPLITAT_H_

#include "2DUniverse/Controller/Controller.h"

#include "2DUniverse/Controller/SplitAt/SplitOptionsAt_uic.h"

#include "2DUniverse/Base/Vector.h"

#include <QtCore/QObject>

#include <vector>

class Point;
class Line;
class QuadraticBezier;
class CubicBezier;

class QSlider;
class QComboBox;

enum E_SplitType
{
    E_ST_FIRST,
    E_ST_LINE,
    E_ST_QBEZIER,
    E_ST_CBEZIER,
    E_ST_LAST,
};

class ControllerSplitAt : public QObject, public Controller
{
    Q_OBJECT;

private:
    typedef std::vector<Point*> PointVector;

public:
    ControllerSplitAt();
    virtual ~ControllerSplitAt();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

protected:
    virtual void init() final override;
    virtual void activate() final override;
    virtual void deactivate() final override;

    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    Point* const getPointAtPos(const Vec2d& pos, size_t* const idx) const;

    void setSliderSplit(const float& s);
    
    float sliderValueToValue(QSlider* const sld, const float& min, const float& max);
    int valueToSliderValue(const float& value, QSlider* const sld, const float& min, const float& max);

private slots:
    bool addPoint(const Vec2d& pos);

    void updateVisibility();

    void setLineVisible(const bool& visible);
    void setQBezVisible(const bool& visible);
    void setCBezVisible(const bool& visible);

    void onSliderValueChanged(int value);
    void onTypeChanged(int idx);

    void updateSplit();

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2d m_CurrentHitDistance;

    PointVector m_LinePoints[3];
    PointVector m_QBezierPoints[3];
    PointVector m_CBezierPoints[3];

    size_t m_CurrentNumMaxPoints;
    size_t m_CurrentNumPoints;

    Line* m_Line[3];
    QuadraticBezier* m_QBezier[3];
    CubicBezier* m_CBezier[3];

    Ui_ControllerSplitAtOptionsWidget m_OptionWidgetSetUp;

    QSlider* m_SplitSlider;
    QComboBox* m_TypeCbb;

    E_SplitType m_SplitType;

    bool m_Splitable;

    const float m_SplitMin;
    const float m_SplitMax;

    const Vec4f m_ColourOrg;
    const Vec4f m_ColourL;
    const Vec4f m_ColourR;
};

#endif //_CONTROLLER_CONTROLLERSPLINEAT_H_
