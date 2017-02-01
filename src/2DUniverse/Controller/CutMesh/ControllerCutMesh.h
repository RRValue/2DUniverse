#ifndef _CONTROLLER_CONTROLLERCUTMESHLINE_H_
#define _CONTROLLER_CONTROLLERCUTMESHLINE_H_

#include "2DUniverse/Controller/Controller.h"

#include <QtCore/QObject>

class Point;
class Line;
class QuadraticBezier;
class CubicBezier;
class Spline;
class HESMesh;

class HESBuilder;
class HESCutter;
class HESCheck;

class QComboBox;

class ControllerCutMesh : public QObject, public Controller
{
    Q_OBJECT;

private:
    enum CutMeshMode
    {
        CMM_FIRST,
        CMM_LINE,
        CMM_QBEZIER,
        CMM_CBEZIER,
        CMM_SPLINE,
        CMM_LAST
    };

private:
    typedef std::vector<HESMesh*> HESMeshVector;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    ControllerCutMesh();
    virtual ~ControllerCutMesh();

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
    void cut();

private slots:
    void onMeshSelectionChanged(int value);
    void onShapeSelectionChanged(int value);
    void onSmallTrianglePreventionChanged(int state);
    void onCutPressed();

private:
    QComboBox* m_CbMeshSelector;
    QComboBox* m_CbShapeSelector;

    CutMeshMode m_CutMode;

    HESMeshVector m_Meshes;
    HESMeshVector m_CutMeshes;
    
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2d m_CurrentHitDistance;

    std::vector<Point*> m_LinePoints;
    std::vector<Point*> m_QBezierPoints;
    std::vector<Point*> m_CBezierPoints;
    std::vector<Point*> m_SplinePoints;

    static const size_t m_NumLinePoints = 2;
    static const size_t m_NumQBezierPoints = 3;
    static const size_t m_NumCBezierPoints = 4;
    static const size_t m_NumSplinePoints = 10;

    std::vector<Point*> m_CutPoints;
    std::vector<Line*> m_CutLines;

    Line* m_Line;
    QuadraticBezier* m_QBezier;
    CubicBezier* m_CBezier;
    Spline* m_Spline;
    
    HESBuilder* m_MeshBuilder;
    HESCutter* m_MeshCutter;
    HESCheck* m_MeshChecker;
};

#endif //_CONTROLLER_CONTROLLERCUTMESHLINE_H_
