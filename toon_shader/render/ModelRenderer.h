#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>
#include <ituGL/shader/Material.h>

#include "../settings/ToonSettings.h"
#include "../util/TextureUtils.h"

class ModelRenderer {

public:
    void Initialize();
    void LoadModel(const char* path);
    void Render(const Camera& camera, const ToonSettings& settings);
    void RebuildToonRamps(const ToonSettings &settings);

private:
    void ApplyToonShader();

    Model m_model;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
    std::shared_ptr<Material> m_material;
    std::vector<glm::vec3> m_materialBaseColors;

    ShaderProgram::Location m_worldMatrixLocation;
    ShaderProgram::Location m_viewProjMatrixLocation;
    ShaderProgram::Location m_lightPositionLocation;

};
