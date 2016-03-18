#ifndef _HALFEDGE_APPLICATION_H_
#define _HALFEDGE_APPLICATION_H_

#include <QtWidgets/QApplication>

#include "HalfEdge2D/MainWindow_uic.h"

class QPaintTarget;
class ViewPort;
class Camera;
class Renderer;
class HESMesh;
class QComboBox;
class Navigator;
class Controller;
class ControllerBuildMesh;
class ControllerShowRings;
class EventHandler;
class Scene;

class HalfEdge2DApplication : public QApplication
{
    Q_OBJECT

public:
    HalfEdge2DApplication() = delete;
    HalfEdge2DApplication(int& argc, char** argv);
    ~HalfEdge2DApplication();

public slots:
    void onRun();

private slots:
    void onMultiViewChanged(int state);
    void onHSliderChanged(int value);
    void onVSliderChanged(int value);
    void onMeshSelectionChanged(int value);
    void onControllerSelectionChanged(const QString& text);

private:
    void init();

    void createGui();
    void createRendering();
    void createViewPorts();

    void setUpMultiView();
    void updateViwePortPartions();

private:
    Ui_HalfEdgeAppMainWindow m_MainWindowForm;
    QWidget* m_MainWidget;
    QComboBox* m_CbController;
    
    // rendering
    QPaintTarget* m_RenderTarget;

    QSlider* m_SldHPart;
    QSlider* m_SldVPart;

    Renderer* m_Renderer;

    bool m_MultiView;

    Scene* m_Scene;

    ViewPort* m_ViewPort0;
    ViewPort* m_ViewPort1;
    ViewPort* m_ViewPort2;
    ViewPort* m_ViewPort3;

    Camera* m_CamVp0;
    Camera* m_CamVp1;
    Camera* m_CamVp2;
    Camera* m_CamVp3;

    EventHandler* m_EventHandler;

    Navigator* m_Navigator;
    ControllerBuildMesh* m_ControllerBuildMesh;
    ControllerShowRings* m_ControllerShowRings;

    std::map<std::string, Controller* const> m_Controller;

    float m_HPartition;
    float m_VPartition;

    // scene
    HESMesh* m_Mesh;
};

#endif //_HALFEDGE_APPLICATION_H_
