#ifndef _SCENE_SCENE_H_
#define _SCENE_SCENE_H_

#include <QtCore/QSize>

class Camera;

class Scene
{
public:
    Scene();
    ~Scene();

    Camera* const getCamera() const;
    void setCamera(Camera* const camera);

private:
    Camera* m_Camera;
};

#endif //_SCENE_SCENE_H_
