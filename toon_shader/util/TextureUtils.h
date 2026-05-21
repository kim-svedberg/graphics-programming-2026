#pragma once

#include <memory>
#include <ituGL/texture/Texture2DObject.h>
#include <stb_image.h>


#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>
#include "../ui/ToonUI.h"

std::shared_ptr<Texture2DObject> CreateToonRampTexture(
    const glm::vec3& shadowColor,
    const glm::vec3& litColor);

glm::vec3 SampleAverageTextureColor(const char* path);