#include "ViewerApplication.h"

#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/asset/ModelLoader.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/shader/Material.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>
#include <stb_image.h> 
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

ViewerApplication::ViewerApplication()
    : Application(1024, 1024, "Toon Shader Demo")
    , m_mainCamera(GetMainWindow())
{
}

void ViewerApplication::Initialize()
{
    Application::Initialize();

    // Initialize DearImGUI
    m_imGui.Initialize(GetMainWindow());

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_mainCamera.InitializeCamera(GetMainWindow());
    m_modelRenderer.Initialize();
    m_modelRenderer.LoadModel("models/mill/Mill.obj"); // change to "models/miku/Default.obj" for Miku model
    m_lightingSystem.InitializeLights();
    m_settingsManager.LoadSettings(m_settings, m_lightingSystem, m_modelRenderer); //Load user-set settings

    DeviceGL& device = GetDevice();
    device.EnableFeature(GL_DEPTH_TEST);
    device.SetVSyncEnabled(true);
}

void ViewerApplication::Update()
{
    Application::Update();

    // Update camera controller
    m_mainCamera.Update(GetMainWindow(), GetDeltaTime());
}

void ViewerApplication::Render()
{
    Application::Render();

    // Clear color and depth
    GetDevice().Clear(true, Color(1.0f, 1.0f, 1.0f, 1.0f), true, 1.0f);

    m_modelRenderer.Render(m_mainCamera.GetCamera(), m_settings);

    // Render the debug user interface
    m_ui.RenderGUI(m_lightingSystem, m_modelRenderer, m_settings, m_settingsManager);
}

void ViewerApplication::Cleanup()
{
    // Cleanup DearImGUI
    m_imGui.Cleanup();

    Application::Cleanup();
}