#include "TextureUtils.h"


std::shared_ptr<Texture2DObject> CreateToonRampTexture(
    const glm::vec3& shadowColor,
    const glm::vec3& litColor)
{
    auto texture = std::make_shared<Texture2DObject>();

    std::vector<std::byte> pixels =
    {
        std::byte(static_cast<unsigned char>(glm::clamp(shadowColor.r, 0.0f, 1.0f) * 255.0f)),
        std::byte(static_cast<unsigned char>(glm::clamp(shadowColor.g, 0.0f, 1.0f) * 255.0f)),
        std::byte(static_cast<unsigned char>(glm::clamp(shadowColor.b, 0.0f, 1.0f) * 255.0f)),
        std::byte(255),

        std::byte(static_cast<unsigned char>(glm::clamp(litColor.r, 0.0f, 1.0f) * 255.0f)),
        std::byte(static_cast<unsigned char>(glm::clamp(litColor.g, 0.0f, 1.0f) * 255.0f)),
        std::byte(static_cast<unsigned char>(glm::clamp(litColor.b, 0.0f, 1.0f) * 255.0f)),
        std::byte(255)
    };

    texture->Bind();

    texture->SetImage(
        0,
        2,
        1,
        TextureObject::FormatRGBA,
        TextureObject::InternalFormatRGBA8,
        std::span<const std::byte>(pixels),
        Data::Type::UByte
    );

    texture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_NEAREST);
    texture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_NEAREST);
    texture->SetParameter(TextureObject::ParameterEnum::WrapS, GL_CLAMP_TO_EDGE);
    texture->SetParameter(TextureObject::ParameterEnum::WrapT, GL_CLAMP_TO_EDGE);

    return texture;
}

