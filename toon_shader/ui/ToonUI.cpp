#include "ToonUI.h"
#include "../render/ModelRenderer.h"
#include "../settings/SettingsManager.h"




void ToonUI::RenderGUI(LightingSystem& lightingSystem, ModelRenderer& modelRenderer, ToonSettings& settings, SettingsManager& settingsManager)
{
    m_imGui.BeginFrame();

    ImGui::Text("Toon Shader Controls");
    ImGui::Separator();

    ImGui::Text("Lighting");
    glm::vec3 lightPos = lightingSystem.GetLightPosition();

    if (ImGui::DragFloat3("Light position", &lightPos[0], 0.1f))
    {
        lightingSystem.SetLightPosition(lightPos);
        settings.lightPosition = lightPos;
    }
    ImGui::Separator();

    ImGui::Text("Toon Ramp");

    bool rampChanged = false;

    rampChanged |= ImGui::Checkbox(
        "Use default toon ramps",
        &settings.useDefaultColorRamps
    );

    if (settings.useDefaultColorRamps)
    {
        rampChanged |= ImGui::DragFloat(
            "Shadow multiplier",
            &settings.shadowStrength,
            0.01f,
            0.0f,
            1.0f
        );

        rampChanged |= ImGui::DragFloat(
            "Highlight multiplier",
            &settings.highlightStrength,
            0.01f,
            1.0f,
            3.0f
        );
    }
    else
    {
        rampChanged |= ImGui::ColorEdit3("Shadow color", &settings.shadowColor[0]);
        rampChanged |= ImGui::ColorEdit3("Lit color", &settings.litColor[0]);
    }

    if (rampChanged)
    {
        modelRenderer.RebuildToonRamps(settings);
    }
    
    ImGui::Separator();

    ImGui::Text("Outline");

    ImGui::DragFloat(
        "Outline thickness",
        &settings.outlineThickness,
        0.001f,
        0.0f,
        0.2f
    );

    ImGui::ColorEdit3(
        "Outline color",
        &settings.outlineColor[0]
    );

    ImGui::Separator();

    if (ImGui::Button("Save Settings"))
    {
        settingsManager.SaveSettings(settings, lightingSystem);
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Defaults"))
    {
        settingsManager.ResetSettings(settings, lightingSystem, modelRenderer);
    }

    ImGui::Separator();

    ImGui::Text("Debug");
    ImGui::Text("Current mode: %s", settings.useDefaultColorRamps ? "Material ramps" : "Custom ramp");
    ImGui::Text("Toon coordinate: max(dot(N, L), 0)");

    m_imGui.EndFrame();
}