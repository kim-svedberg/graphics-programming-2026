#pragma once

#include "../settings/ToonSettings.h"
#include <ituGL/utils/DearImGui.h>
#include "../light/LightingSystem.h"
#include <imgui.h>

class ModelRenderer;
class SettingsManager;

class ToonUI {
private:

public:
    void RenderGUI(LightingSystem& lightingSystem, ModelRenderer& modelRenderer, ToonSettings& settings);

private:
    // Helper object for debug GUI
    DearImGui m_imGui;

    SettingsManager m_settingsManager;
    
};
