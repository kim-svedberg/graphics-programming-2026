#pragma once

#include <ituGL/application/Application.h>

#include "camera/ToonCamera.h"
#include "light/LightingSystem.h"

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>


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
    void InitializeLights();

    void RenderGUI();

    void ApplyToonShader(Model &model);
    void RebuildToonRamps();
    void SaveSettings();
    void LoadSettings();
    void ResetSettings();


private:
    // Helper object for debug GUI
    DearImGui m_imGui;

    // Loaded model
    Model m_model;

    // Camera
    ToonCamera m_mainCamera;

    // Light variables
    LightingSystem m_lightingSystem;

    //Textures for toon ramp
    std::vector<std::string> texturePaths;

    // Variables for toon shading 
    std::vector<glm::vec3> m_materialBaseColors;
    glm::vec3 m_toonShadowColor = glm::vec3(0.15f);
    glm::vec3 m_toonLitColor = glm::vec3(1.0f);
    float m_toonShadowStrength = 0.35f;
    float m_toonHighlightStrength = 1.25f;
    bool m_useMaterialColorRamps = true;

private:
    struct ToonSettings
    {
        bool useMaterialColorRamps = true;

        float toonShadowStrength = 0.5f;
        float toonHighlightStrength = 1.5f;

        glm::vec3 toonShadowColor = glm::vec3(0.2f);
        glm::vec3 toonLitColor = glm::vec3(1.0f);

        glm::vec3 lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
    };
};
