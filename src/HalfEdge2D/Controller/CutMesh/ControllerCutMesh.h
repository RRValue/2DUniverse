#ifndef _CONTROLLER_CONTROLLERCUTMESH_H_
#define _CONTROLLER_CONTROLLERCUTMESH_H_

#include "HalfEdge2D/Controller/Controller.h"

#include <QtCore/QObject>

class QSlider;

class ControllerCutMesh : public QObject, public Controller
{
    Q_OBJECT;

public:
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
};

#endif //_CONTROLLER_CONTROLLERCUTMESH_H_
