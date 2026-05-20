#include <ituGL/application/Application.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>

class LightingSystem{

public:
    void InitializeLights();
private:

public:
private:
    glm::vec3 m_lightPosition;

};