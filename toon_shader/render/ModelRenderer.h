#pragma once

#include <ituGL/application/Application.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>

class ModelRenderer {

public:
    void Initialize();
    void Render(const Model&, const Camera&, const glm::vec3& lightPos);
    void SetToonSettings(...);

private:
    
};
