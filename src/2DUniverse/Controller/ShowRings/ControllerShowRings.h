#ifndef _CONTROLLER_CONTROLLERSHOWRINGS_H_
#define _CONTROLLER_CONTROLLERSHOWRINGS_H_

#include "2DUniverse/Controller/Controller.h"

#include <QtCore/QObject>

#include <vector>

class Scene;
class Renderer;
class ViewPort;
class QPaintTarget;
class Face;
class HESMesh;
class HESFace;

class QComboBox;

typedef std::vector<HESFace*> FaceVector;
typedef std::vector<FaceVector> RingFacesVector;

class LastFaceColour
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    HESFace* m_Face;
    Vec4f m_Colour;
};

typedef std::vector<LastFaceColour, Eigen::aligned_allocator<LastFaceColour>> LastColourVector;

class ControllerShowRings : public QObject, public Controller
{
    Q_OBJECT;

public:
    ControllerShowRings();
    virtual ~ControllerShowRings();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:
    virtual void init() final override;
    virtual void activate() final override;
    virtual void deactivate() final override;

    virtual bool handleMouseMoveEvent(QMouseEvent* const event) final override;
    virtual bool handleMousePressEvent(QMouseEvent* const event) final override;
    virtual bool handleMouseReleaseEvent(QMouseEvent* const event) final override;
    virtual bool handleResizeEvent(QResizeEvent* const event) final override;
    virtual bool handleWheelEvent(QWheelEvent* const event) final override;

protected:
    virtual void postSceneChanged() final override;

private:
    void updateIdTarget();

    Vec4f idToColour(const unsigned int& id);
    unsigned int colourToId(const Vec4f& colour);

    void clear();

    void unsetLastFaces();
    RingFacesVector findRingOrder(HESFace* const face, const size_t& order);
    void colourRingFaces(const RingFacesVector& ringFaces);

private slots:
    void onMeshSelectionChanged(int value);

private:
    const size_t m_ChannelBitRange;
    const size_t m_ChannelRange;
    const size_t m_MaxId;
    const float m_ChannelFFactor;
    const size_t m_NumRings;

private:
    Scene* m_IdScene;

    QPaintTarget* m_IdTarget;
    
    Renderer* m_IdRenderer;
    ViewPort* m_ViewPort;

    std::vector<Face*> m_Faces;

    Mat3f m_MousePosToTargetMat;

    size_t m_LastHitId;
    LastColourVector m_LastColourVector;

    QComboBox* m_CbMeshSelector;

    HESMesh* m_Mesh;
};

#endif //_CONTROLLER_CONTROLLERSHOWRINGS_H_
