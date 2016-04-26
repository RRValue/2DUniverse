#ifndef _CONTROLLER_CONTROLLERSPLITAT_H_
#define _CONTROLLER_CONTROLLERSPLITAT_H_

#include "HalfEdge2D/Controller/Controller.h"

#include "HalfEdge2D/Controller/SplitAt/SplitOptionsAt_uic.h"

#include <HalfEdge2D/Base/Vector.h>

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
    typedef std::vector<Point* const> PointVector;

public:
    ControllerSplitAt();
    virtual ~ControllerSplitAt();

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
    Point* const getPointAtPos(const Vec2f& pos, size_t* const idx) const;

    void setSliderSplit(const float& s);
    
    float sliderValueToValue(QSlider* const sld, const float& min, const float& max);
    int valueToSliderValue(const float& value, QSlider* const sld, const float& min, const float& max);

private slots:
    bool addPoint(const Vec2f& pos);

    void updateVisibility();

    void setLineVisible(const bool& visible);
    void setQBezVisible(const bool& visible);
    void setCBezVisible(const bool& visible);

    void onSliderMoved(int value);
    void onTypeChanged(int idx);

    void updateSplit();

private:
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2f m_CurrentHitDistance;

    PointVector m_LinePoints;
    PointVector m_SplitLinePoints;
    
    PointVector m_QBezierPoints;
    PointVector m_SplitQBezierPoints;
    
    PointVector m_CBezierPoints;
    PointVector m_SplitCBezierPoints;

    size_t m_CurrentNumMaxPoints;
    size_t m_CurrentNumPoints;

    Line* m_Line;
    Line* m_SplitLine;
    
    QuadraticBezier* m_QBezier;
    QuadraticBezier* m_SplitQBezier;
    
    CubicBezier* m_CBezier;
    CubicBezier* m_SplitCBezier;

    Ui_ControllerSplitAtOptionsWidget m_OptionWidgetSetUp;

    QSlider* m_SplitSlider;
    QComboBox* m_TypeCbb;

    E_SplitType m_SplitType;

    bool m_Splitable;

    const float m_SplitMin;
    const float m_SplitMax;
};

#endif //_CONTROLLER_CONTROLLERSPLINEAT_H_
