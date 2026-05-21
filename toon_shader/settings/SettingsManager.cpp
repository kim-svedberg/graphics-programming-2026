#include "SettingsManager.h"
#include <fstream>
#include "../render/ModelRenderer.h"


void SettingsManager::SaveSettings(ToonSettings &settings, LightingSystem &lightingSystem){

    std::ofstream file("toon_settings.txt");

    if (!file.is_open())
    {
        return;
    }

    file << settings.useDefaultColorRamps << "\n";

    file << settings.shadowStrength << "\n";
    file << settings.highlightStrength << "\n";

    file << settings.shadowColor.r << " "
         << settings.shadowColor.g << " "
         << settings.shadowColor.b << "\n";

    file << settings.litColor.r << " "
         << settings.litColor.g << " "
         << settings.litColor.b << "\n";

    file << lightingSystem.GetLightPosition().x << " "
         << lightingSystem.GetLightPosition().y << " "
         << lightingSystem.GetLightPosition().z << "\n";
    
    file << settings.outlineThickness << "\n";

    file << settings.outlineColor.r << " "
         << settings.outlineColor.g << " "
         << settings.outlineColor.b << "\n";
}

void SettingsManager::LoadSettings(ToonSettings &settings, LightingSystem &lightingSystem, ModelRenderer &modelRenderer){
    std::ifstream file("toon_settings.txt");

    if (!file.is_open())
    {
        return;
    }

    file >> settings.useDefaultColorRamps;

    file >> settings.shadowStrength;
    file >> settings.highlightStrength;

    file >> settings.shadowColor.r
         >> settings.shadowColor.g
         >> settings.shadowColor.b;

    file >> settings.litColor.r
         >> settings.litColor.g
         >> settings.litColor.b;

    glm::vec3 pos;
    file >> pos.x >> pos.y >> pos.z;
    lightingSystem.SetLightPosition(pos);

    file >> settings.outlineThickness;

    file >> settings.outlineColor.r
         >> settings.outlineColor.g
         >> settings.outlineColor.b;

    modelRenderer.RebuildToonRamps(settings);
}

void SettingsManager::ResetSettings(ToonSettings &settings, LightingSystem &lightingSystem, ModelRenderer &modelRenderer){
    settings.useDefaultColorRamps = true;

    settings.shadowStrength = 0.5f;
    settings.highlightStrength = 1.5f;

    settings.shadowColor = glm::vec3(0.2f);
    settings.litColor = glm::vec3(1.0f);

    lightingSystem.SetLightPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    settings.outlineThickness = 0.05f;
    settings.outlineColor = glm::vec3(0.0f);

    modelRenderer.RebuildToonRamps(settings);
}