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
    , m_lightingSystem(glm::vec3(0.0f))
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
    m_modelRenderer.LoadModel("models/miku/Default.obj");
    m_lightingSystem.InitializeLights();
    LoadSettings(); //Load user-set settings

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
    RenderGUI();
}

void ViewerApplication::Cleanup()
{
    // Cleanup DearImGUI
    m_imGui.Cleanup();

    Application::Cleanup();
}

glm::vec3 SampleAverageTextureColor(const char* path)
{
    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    if (!data)
    {
        return glm::vec3(1.0f); // fallback white
    }

    glm::vec3 averageColor(0.0f);
    int pixelCount = width * height;

    for (int i = 0; i < pixelCount; i++)
    {
        int index = i * 4;

        glm::vec3 pixelColor(
            data[index + 0] / 255.0f,
            data[index + 1] / 255.0f,
            data[index + 2] / 255.0f
        );

        averageColor += pixelColor;
    }

    averageColor /= float(pixelCount);

    stbi_image_free(data);

    return averageColor;
}

void ViewerApplication::SaveSettings()
{
    std::ofstream file("toon_settings.txt");

    if (!file.is_open())
    {
        return;
    }

    file << m_settings.useMaterialColorRamps << "\n";

    file << m_settings.shadowStrength << "\n";
    file << m_settings.highlightStrength << "\n";

    file << m_settings.shadowColor.r << " "
         << m_settings.shadowColor.g << " "
         << m_settings.shadowColor.b << "\n";

    file << m_settings.litColor.r << " "
         << m_settings.litColor.g << " "
         << m_settings.litColor.b << "\n";

    file << m_lightingSystem.GetLightPosition().x << " "
         << m_lightingSystem.GetLightPosition().y << " "
         << m_lightingSystem.GetLightPosition().z << "\n";
}

void ViewerApplication::LoadSettings()
{
    std::ifstream file("toon_settings.txt");

    if (!file.is_open())
    {
        return;
    }

    file >> m_settings.useMaterialColorRamps;

    file >> m_settings.shadowStrength;
    file >> m_settings.highlightStrength;

    file >> m_settings.shadowColor.r
         >> m_settings.shadowColor.g
         >> m_settings.shadowColor.b;

    file >> m_settings.litColor.r
         >> m_settings.litColor.g
         >> m_settings.litColor.b;

    glm::vec3 pos;
    file >> pos.x >> pos.y >> pos.z;
    m_lightingSystem.SetLightPosition(pos);

    m_modelRenderer.RebuildToonRamps(m_settings);
}

void ViewerApplication::ResetSettings()
{
    m_settings.useMaterialColorRamps = true;

    m_settings.shadowStrength = 0.5f;
    m_settings.highlightStrength = 1.5f;

    m_settings.shadowColor = glm::vec3(0.2f);
    m_settings.litColor = glm::vec3(1.0f);

    m_lightingSystem.SetLightPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    m_modelRenderer.RebuildToonRamps(m_settings);
}

void ViewerApplication::RenderGUI()
{
    m_imGui.BeginFrame();

    ImGui::Text("Toon Shader Controls");
    ImGui::Separator();

    ImGui::Text("Lighting");
    ImGui::DragFloat3("Light position", &m_lightingSystem.GetLightPosition()[0], 0.1f);

    ImGui::Separator();

    ImGui::Text("Toon Ramp");

    bool rampChanged = false;

    rampChanged |= ImGui::Checkbox(
        "Use material-based ramps",
        &m_settings.useMaterialColorRamps
    );

    if (m_settings.useMaterialColorRamps)
    {
        rampChanged |= ImGui::DragFloat(
            "Shadow multiplier",
            &m_settings.shadowStrength,
            0.01f,
            0.0f,
            1.0f
        );

        rampChanged |= ImGui::DragFloat(
            "Highlight multiplier",
            &m_settings.highlightStrength,
            0.01f,
            1.0f,
            3.0f
        );
    }
    else
    {
        rampChanged |= ImGui::ColorEdit3("Shadow color", &m_settings.shadowColor[0]);
        rampChanged |= ImGui::ColorEdit3("Lit color", &m_settings.litColor[0]);
    }

    if (rampChanged)
    {
        m_modelRenderer.RebuildToonRamps(m_settings);
    }

    ImGui::Separator();

    if (ImGui::Button("Save Settings"))
    {
        SaveSettings();
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Defaults"))
    {
        ResetSettings();
    }

    ImGui::Separator();

    ImGui::Text("Debug");
    ImGui::Text("Current mode: %s", m_settings.useMaterialColorRamps ? "Material ramps" : "Custom ramp");
    ImGui::Text("Toon coordinate: max(dot(N, L), 0)");

    m_imGui.EndFrame();
}
