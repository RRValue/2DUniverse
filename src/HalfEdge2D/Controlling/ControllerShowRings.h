#ifndef _CONTROLLING_CONTROLLERSHOWRINGS_H_
#define _CONTROLLING_CONTROLLERSHOWRINGS_H_

#include "HalfEdge2D/Controlling/Controller.h"

#include <vector>
#include <map>
#include <set>

class HESMesh;
class HESFace;
class Scene;
class Renderer;
class ViewPort;
class QPaintTarget;
class Face;

class ControllerShowRings : public Controller
{
public:
    ControllerShowRings();
    virtual ~ControllerShowRings();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    void setMesh(HESMesh* const mesh);

protected:
    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

private:
    void updateIdTarget();

    Vec4f idToColour(const unsigned int& id);
    unsigned int colourToId(const Vec4f& colour);

    void clear();

    void unsetLastFaces();
    std::set<HESFace* const> findRing(HESFace* const face);
    void setNeigbourFaces(HESFace* const centerface, const std::set<HESFace* const>& ringFaces);

private:
    const size_t m_ChannelBitRange;
    const size_t m_ChannelRange;
    const size_t m_MaxId;
    const float m_ChannelFFactor;

    const Vec4f m_FaceHitColour;
    const Vec4f m_FaceRingColour;

private:
    Scene* m_Scene;
    HESMesh* m_Mesh;

    QPaintTarget* m_IdTarget;
    
    Renderer* m_IdRenderer;
    ViewPort* m_ViewPort;

    std::vector<Face*> m_Faces;

    Mat3f m_MousePosToTargetMat;

    size_t m_LastHitId;
    std::map<HESFace* const, Vec4f, std::less<HESFace* const>, Eigen::aligned_allocator<std::pair<HESFace* const, Vec4f>>> m_LastFacesColours;
    std::set<HESFace* const> m_LastFaces;
};

#endif //_CONTROLLING_CONTROLLERSHOWRINGS_H_
