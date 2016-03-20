#ifndef _CONTROLLING_CONTROLLERSHOWRINGS_H_
#define _CONTROLLING_CONTROLLERSHOWRINGS_H_

#include "HalfEdge2D/Controlling/Controller.h"

#include <vector>

class HESMesh;
class Scene;
class Renderer;
class ViewPort;
class QPaintTarget;
class Triangle;

class ControllerShowRings : public Controller
{
public:
    ControllerShowRings();
    virtual ~ControllerShowRings();

    void setMesh(HESMesh* const mesh);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    void updateIdTarget();

private:
    Scene* m_Scene;
    HESMesh* m_Mesh;

    QPaintTarget* m_IdTarget;
    
    Renderer* m_Renderer;
    ViewPort* m_ViewPort;

    std::vector<Triangle*> m_Triangles;
};

#endif //_CONTROLLING_CONTROLLERSHOWRINGS_H_
