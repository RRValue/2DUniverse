#ifndef _SCENE_CANVAS_H_
#define _SCENE_CANVAS_H_

#include <QtCore/QSize>

class Canvas
{
public:
    Canvas();
    ~Canvas();

    // getter
    const QSize& getSize() const;
    const float& getAspectRatio() const;

    // setter
    void setSize(const QSize& size);

private:
    QSize m_Size;
    float m_AspectRatio;
};

#endif //_SCENE_CANVAS_H_
