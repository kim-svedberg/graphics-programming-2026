#pragma once

#include <ituGL/application/Application.h>

#include "camera/ToonCamera.h"
#include "light/LightingSystem.h"
#include "util/TextureUtils.h"
#include "render/ModelRenderer.h"
#include "settings/ToonSettings.h"
#include "ui/ToonUI.h"

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

    void RenderGUI();
    void SaveSettings();
    void LoadSettings();
    void ResetSettings();


private:
    // Helper object for debug GUI
    DearImGui m_imGui;

    // Loaded model
    ModelRenderer m_modelRenderer;

    // Camera
    ToonCamera m_mainCamera;

    // Light variables
    LightingSystem m_lightingSystem;

    // Settings
    ToonSettings m_settings;
    
    // UI 
    ToonUI m_ui;

};
