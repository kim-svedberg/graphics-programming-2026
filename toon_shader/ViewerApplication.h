#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>

class Texture2DObject;

class ViewerApplication : public Application
{
public:
    ViewerApplication();

protected:
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Cleanup() override;

private:
    void InitializeModel();
    void InitializeCamera();
    void InitializeLights();

    void UpdateCamera();

    void RenderGUI();

    void RebuildToonRamps();


private:
    // Helper object for debug GUI
    DearImGui m_imGui;

    // Mouse position for camera controller
    glm::vec2 m_mousePosition;

    // Camera controller parameters
    Camera m_camera;
    glm::vec3 m_cameraPosition;
    float m_cameraTranslationSpeed;
    float m_cameraRotationSpeed;
    bool m_cameraEnabled;
    bool m_cameraEnablePressed;

    // Loaded model
    Model m_model;

    // Add light variables
    glm::vec3 m_ambientColor;
    glm::vec3 m_lightColor;
    float m_lightIntensity;
    glm::vec3 m_lightPosition;

    // Specular exponent debug
    float m_specularExponentGrass;

    // Variables for toon shading 
    std::vector<glm::vec3> m_materialBaseColors;
    glm::vec3 m_toonShadowColor = glm::vec3(0.15f);
    glm::vec3 m_toonLitColor = glm::vec3(1.0f);
    float m_toonShadowStrength = 0.35f;
    float m_toonHighlightStrength = 1.25f;
    bool m_useMaterialColorRamps = true;
};
