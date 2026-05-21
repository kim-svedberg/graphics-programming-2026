#pragma once

#include "ToonSettings.h"
#include "../light/LightingSystem.h"

class ModelRenderer;

class SettingsManager {
public:
    void SaveSettings(ToonSettings &settings, LightingSystem &lightingSystem);
    void LoadSettings(ToonSettings &settings, LightingSystem &lightingSystem, ModelRenderer &modelRenderer);
    void ResetSettings(ToonSettings &settings, LightingSystem &lightingSystem, ModelRenderer &modelRenderer);
};