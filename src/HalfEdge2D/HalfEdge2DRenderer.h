#ifndef _HALFEDGE_RENDERER_H_
#define _HALFEDGE_RENDERER_H_

class QPaintEvent;

class QWidget;
class Scene;

class HalfEdge2DRenderer
{
public:
    HalfEdge2DRenderer();
    ~HalfEdge2DRenderer();

    void render(QPaintEvent* const event);

    void setWidget(QWidget* const widget);
    void setScene(Scene* const scene);

private:
    QWidget* m_Widget;
    Scene* m_Scene;
};

#endif //_HALFEDGE_WIDGET_H_
