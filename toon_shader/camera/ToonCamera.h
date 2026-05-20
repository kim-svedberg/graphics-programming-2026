#pragma once

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/texture/Texture2DObject.h>
#include <ituGL/core/DeviceGL.h>
#include <ituGL/application/Window.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

class ToonCamera
{

public:
    ToonCamera(Window& window);
    void InitializeCamera(Window &window);
    void Update(Window& window, float dt);
    const Camera& GetCamera() const;
    
private:
    // Mouse position for camera controller
    glm::vec2 m_mousePosition;

    // Camera controller parameters
    Camera m_camera;
    glm::vec3 m_cameraPosition;
    float m_cameraTranslationSpeed;
    float m_cameraRotationSpeed;
    bool m_cameraEnabled;
    bool m_cameraEnablePressed;
};