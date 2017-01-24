#ifndef _CONTROLLER_CONTROLLERCUTCIRCLE_H_
#define _CONTROLLER_CONTROLLERCUTCIRCLE_H_

#include "HalfEdge2D/Controller/Controller.h"

#include "HalfEdge2D/Controller/CutCircle/CutCircleOption_uic.h"

#include <HalfEdge2D/Base/Vector.h>

#include <QtCore/QObject>

#include <vector>
#include <array>

class Point;
class Line;
class Circle;

class QSlider;

class ControllerCutCircle : public QObject, public Controller
{
    Q_OBJECT;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    ControllerCutCircle();
    virtual ~ControllerCutCircle();

protected:
    virtual void init() final override;
    virtual void activate() final override;
    virtual void deactivate() final override;

    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private slots:
    void onRadiusSliderValueChanged(int value);

private:
    Point* const getPointAtPos(const Vec2d& pos, size_t* const idx) const;
    void cut();
    void setSliderRadius(const float& radius);
    float sliderValueToRadius(const int& value);
    int radiusToSliderValue(const float& value);

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2d m_CurrentHitDistance;

    std::vector<Point*> m_Points;
    Line* m_Line;
    Circle* m_Circle;

    Ui_ControllerCutCircleOptionsWidget m_OptionWidgetSetUp;
    QSlider* m_RadiusSlider;

    const float m_RadusMin;
    const float m_RadusMax;

    Point* m_CutPoint0;
    Point* m_CutPoint1;
};

#endif //_CONTROLLER_CONTROLLERCUTCIRCLE_H_
