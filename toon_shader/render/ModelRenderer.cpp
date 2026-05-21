#include "ModelRenderer.h"
#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/shader/ShaderUniformCollection.h>
#include <ituGL/asset/ModelLoader.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

void ModelRenderer::Initialize(){

    Shader vertexShader =
        ShaderLoader::Load(Shader::VertexShader, "shaders/toon.vert");

    Shader fragmentShader =
        ShaderLoader::Load(Shader::FragmentShader, "shaders/toon.frag");

    m_shaderProgram = std::make_shared<ShaderProgram>();
    m_shaderProgram->Build(vertexShader, fragmentShader);

    m_worldMatrixLocation = m_shaderProgram->GetUniformLocation("WorldMatrix");
    m_viewProjMatrixLocation = m_shaderProgram->GetUniformLocation("ViewProjMatrix");
    m_lightPositionLocation = m_shaderProgram->GetUniformLocation("LightPosition");

    m_isOutlineLocation = m_shaderProgram->GetUniformLocation("IsOutline");
    m_outlineThicknessLocation = m_shaderProgram->GetUniformLocation("OutlineThickness");
    m_outlineColorLocation = m_shaderProgram->GetUniformLocation("OutlineColor");

    ShaderUniformCollection::NameSet filteredUniforms;
    filteredUniforms.insert("WorldMatrix");
    filteredUniforms.insert("ViewProjMatrix");
    filteredUniforms.insert("LightPosition");
    filteredUniforms.insert("IsOutline");
    filteredUniforms.insert("OutlineThickness");
    filteredUniforms.insert("OutlineColor");

    m_material = std::make_shared<Material>(m_shaderProgram, filteredUniforms);
    m_material->SetUniformValue("Color", glm::vec4(1.0f));
}

void ModelRenderer::LoadModel(const char* path){
    ModelLoader loader(m_material);
    
    loader.SetCreateMaterials(true);
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Position, "VertexPosition");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Normal, "VertexNormal");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::TexCoord0, "VertexTexCoord");

    loader.SetMaterialProperty(
        ModelLoader::MaterialProperty::DiffuseTexture,
        "ColorTexture"
    );

    loader.GetTexture2DLoader().SetFlipVertical(true);
    m_model = loader.Load(path);

    ApplyToonShader();

}

void ModelRenderer::Render(
    const Camera& camera, const ToonSettings& settings
)
{
    ShaderProgram& shader = *m_shaderProgram;
    shader.Use();

    glm::mat4 worldMatrix = glm::scale(glm::vec3(0.1f));

    shader.SetUniform(
        m_worldMatrixLocation,
        worldMatrix
    );

    shader.SetUniform(
        m_viewProjMatrixLocation,
        camera.GetViewProjectionMatrix()
    );

    shader.SetUniform(
        m_lightPositionLocation,
        settings.lightPosition
    );

    // First pass: outline
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    shader.SetUniform(m_isOutlineLocation, 1);
    shader.SetUniform(m_outlineThicknessLocation, 0.05f);
    shader.SetUniform(m_outlineColorLocation, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    m_model.Draw();

    // Second pass: normal toon model 
    glCullFace(GL_BACK);

    shader.SetUniform(m_isOutlineLocation, 0);

    m_model.Draw();

    glDisable(GL_CULL_FACE);
}

void ModelRenderer::ApplyToonShader()
{
    for (size_t i = 0; i < m_model.GetMaterialCount(); i++)
    {
        glm::vec3 baseColor(0.5f);

        m_materialBaseColors.push_back(baseColor);
        glm::vec3 shadowColor = glm::vec3(0.0); //black 
        glm::vec3 litColor = glm::vec3(1.0); // white

        auto toonRamp = CreateToonRampTexture(shadowColor, litColor);

        m_model.GetMaterial(i).SetUniformValue("ToonRamp", toonRamp);
    }
}

void ModelRenderer::RebuildToonRamps(const ToonSettings& settings){
    for (size_t i = 0; i < m_model.GetMaterialCount(); i++)
    {
        glm::vec3 baseColor = m_materialBaseColors[i];

        glm::vec3 shadowColor;
        glm::vec3 litColor;

        if (settings.useDefaultColorRamps)
        {
            shadowColor = baseColor * settings.shadowStrength;
            litColor = glm::min(
                baseColor * settings.highlightStrength,
                glm::vec3(1.0f)
            );
        }
        else
        {
            shadowColor = settings.shadowColor;
            litColor = settings.litColor;
        }

        auto toonRamp = CreateToonRampTexture(shadowColor, litColor);

        m_model.GetMaterial(i).SetUniformValue("ToonRamp", toonRamp);
    }
}