#ifndef _CONTROLLER_CONTROLLERCUTMESHLINE_H_
#define _CONTROLLER_CONTROLLERCUTMESHLINE_H_

#include "HalfEdge2D/Controller/Controller.h"

#include <QtCore/QObject>

class Point;
class Line;
class HESMesh;

class HESBuilder;
class HESCutter;
class HESCheck;

class QComboBox;

class ControllerCutMeshLine : public QObject, public Controller
{
    Q_OBJECT;

private:
    typedef std::vector<HESMesh*> HESMeshVector;

public:
    ControllerCutMeshLine();
    virtual ~ControllerCutMeshLine();

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
    void cut();

private slots:
    void onMeshSelectionChanged(int value);

private:
    QComboBox* m_CbMeshSelector;

    HESMeshVector m_Meshes;
    HESMeshVector m_CutMeshes;
    
    bool m_MovePoint;

    Point* m_CurrentPoint;
    size_t m_CurrentPointIdx;
    Vec2f m_CurrentHitDistance;

    std::vector<Point* const> m_Points;
    Line* m_Line;

    HESBuilder* m_MeshBuilder;
    HESCutter* m_MeshCutter;
    HESCheck* m_MeshChecker;
};

#endif //_CONTROLLER_CONTROLLERCUTMESHLINE_H_
