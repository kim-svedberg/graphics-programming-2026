#include <ituGL/application/Application.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>

class LightingSystem{

public:
    LightingSystem(glm::vec3 startingValue);

    void InitializeLights();
    const glm::vec3& GetLightPosition() const;
    glm::vec3& GetLightPosition();
    void SetLightPosition(const glm::vec3& pos);

private:

public:
private:
    glm::vec3 m_lightPosition;

};