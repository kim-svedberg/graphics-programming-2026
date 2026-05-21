#pragma once

#include "ToonSettings.h"
#include <imgui.h>
#include <ituGL/utils/DearImGui.h>
#include "../light/LightingSystem.h"
#include "../render/ModelRenderer.h"

class ToonUI {
private:

public:
    void RenderGUI(LightingSystem &lightingSystem, ModelRenderer &modelRenderer, ToonSettings &settings);

private:
    // Helper object for debug GUI
    DearImGui m_imGui;
};
