#ifndef _CONTROLLER_CONTROLLERSPLINEAT_H_
#define _CONTROLLER_CONTROLLERSPLINEAT_H_

#include "2DUniverse/Controller/Controller.h"

#include "2DUniverse/Controller/SplineAt/SplineOptionsAt_uic.h"

#include "2DUniverse/Base/Vector.h"

#include <QtCore/QObject>

#include <vector>

class Point;
class Line;
class Spline;

class QSlider;
class QCheckBox;

class ControllerSplineAt : public QObject, public Controller
{
    Q_OBJECT;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    ControllerSplineAt();
    virtual ~ControllerSplineAt();

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

    void setSliderC(const float& c);
    void setSliderT(const float& t);
    void setSliderB(const float& b);
    void setSliderA(const float& a);

    float sliderValueToValue(QSlider* const sld, const float& min, const float& max);
    int valueToSliderValue(const float& value, QSlider* const sld, const float& min, const float& max);

private slots:
    void onSliderValueChanged(int value);
    void onClosedChanged(int state);

    void updateData();

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2d m_CurrentHitDistance;

    std::vector<Point*> m_Points;
    Spline* m_Spline;
    
    Point* m_Point;
    Line* m_Tangent;
    Line* m_Normal;

    Point* m_PointN;
    Line* m_TangentN;
    Line* m_NormalN;

    Ui_ControllerSplineAtOptionsWidget m_OptionWidgetSetUp;

    QSlider* m_CSlider;
    QSlider* m_TSlider;
    QSlider* m_BSlider;
    QSlider* m_ASlider;
    QCheckBox* m_ClosedCkb;

    const float m_CMin;
    const float m_CMax;
    const float m_TMin;
    const float m_TMax;
    const float m_BMin;
    const float m_BMax;
    const float m_AMin;
    const float m_AMax;
};

#endif //_CONTROLLER_CONTROLLERSPLINEAT_H_
