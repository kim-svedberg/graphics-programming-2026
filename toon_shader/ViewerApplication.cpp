#include "ViewerApplication.h"

#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/asset/ModelLoader.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/shader/Material.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>
#include <stb_image.h> 

ViewerApplication::ViewerApplication()
    : Application(1024, 1024, "Viewer demo")
    , m_cameraPosition(0, 30, 30)
    , m_cameraTranslationSpeed(20.0f)
    , m_cameraRotationSpeed(0.5f)
    , m_cameraEnabled(false)
    , m_cameraEnablePressed(false)
    , m_mousePosition(GetMainWindow().GetMousePosition(true))
    , m_ambientColor(0.0f)
    , m_lightColor(0.0f)
    , m_lightIntensity(0.0f)
    , m_lightPosition(0.0f)
    , m_specularExponentGrass(100.0f)
{
}

void ViewerApplication::Initialize()
{
    Application::Initialize();

    // Initialize DearImGUI
    m_imGui.Initialize(GetMainWindow());

    InitializeModel();
    InitializeCamera();
    InitializeLights();

    DeviceGL& device = GetDevice();
    device.EnableFeature(GL_DEPTH_TEST);
    device.SetVSyncEnabled(true);
}

void ViewerApplication::Update()
{
    Application::Update();

    // Update camera controller
    UpdateCamera();

    //Update eye position for all materials for toon shader
    for (int i = 0; i < m_model.GetMaterialCount(); i++)
{
    m_model.GetMaterial(i).SetUniformValue(
        "EyePosition",
        m_cameraPosition
    );
}
}

void ViewerApplication::Render()
{
    Application::Render();

    // Clear color and depth
    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    m_model.Draw();

    // Render the debug user interface
    RenderGUI();
}

void ViewerApplication::Cleanup()
{
    // Cleanup DearImGUI
    m_imGui.Cleanup();

    Application::Cleanup();
}

glm::vec3 SampleAverageTextureColor(const char* path)
{
    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    if (!data)
    {
        return glm::vec3(1.0f); // fallback white
    }

    glm::vec3 averageColor(0.0f);
    int pixelCount = width * height;

    for (int i = 0; i < pixelCount; i++)
    {
        int index = i * 4;

        glm::vec3 pixelColor(
            data[index + 0] / 255.0f,
            data[index + 1] / 255.0f,
            data[index + 2] / 255.0f
        );

        averageColor += pixelColor;
    }

    averageColor /= float(pixelCount);

    stbi_image_free(data);

    return averageColor;
}

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
    Data::Type::UByte);

    texture->SetParameter(TextureObject::ParameterEnum::MinFilter, GL_NEAREST);
    texture->SetParameter(TextureObject::ParameterEnum::MagFilter, GL_NEAREST);
    texture->SetParameter(TextureObject::ParameterEnum::WrapS, GL_CLAMP_TO_EDGE);
    texture->SetParameter(TextureObject::ParameterEnum::WrapT, GL_CLAMP_TO_EDGE);

    return texture;
}

void ViewerApplication::RebuildToonRamps()
{
    for (size_t i = 0; i < m_model.GetMaterialCount(); i++)
    {
        glm::vec3 baseColor(1.0f);

        if (i == 0)
            baseColor = glm::vec3(0.35f);
        else if (i == 1)
            baseColor = glm::vec3(0.4f, 0.8f, 0.35f);
        else if (i == 2)
            baseColor = glm::vec3(0.8f, 0.65f, 0.45f);

        glm::vec3 shadowColor;
        glm::vec3 litColor;

        if (m_useMaterialColorRamps)
        {
            shadowColor = baseColor * m_toonShadowStrength;
            litColor = glm::min(baseColor * m_toonHighlightStrength, glm::vec3(1.0f));
        }
        else
        {
            shadowColor = m_toonShadowColor;
            litColor = m_toonLitColor;
        }

        auto toonRamp = CreateToonRampTexture(shadowColor, litColor);
        m_model.GetMaterial(i).SetUniformValue("ToonRamp", toonRamp);
    }
}

void ViewerApplication::InitializeModel()
{
    //Load and build toon shader 
    Shader vertexShader = ShaderLoader::Load(Shader::VertexShader, "shaders/toon.vert");
    Shader fragmentShader = ShaderLoader::Load(Shader::FragmentShader, "shaders/toon.frag");
    std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->Build(vertexShader, fragmentShader);

    // Filter out uniforms that are not material properties
    ShaderUniformCollection::NameSet filteredUniforms;
    filteredUniforms.insert("WorldMatrix");
    filteredUniforms.insert("ViewProjMatrix");
    filteredUniforms.insert("AmbientColor");
    filteredUniforms.insert("LightColor");

    // Create reference material
    std::shared_ptr<Material> material = std::make_shared<Material>(shaderProgram, filteredUniforms);
    material->SetUniformValue("Color", glm::vec4(1.0f));
    material->SetUniformValue("AmbientReflection", 1.0f);
    material->SetUniformValue("DiffuseReflection", 1.0f);
    material->SetUniformValue("SpecularReflection", 1.0f);
    material->SetUniformValue("SpecularExponent", 100.0f);

    // Setup function
    ShaderProgram::Location worldMatrixLocation = shaderProgram->GetUniformLocation("WorldMatrix");
    ShaderProgram::Location viewProjMatrixLocation = shaderProgram->GetUniformLocation("ViewProjMatrix");
    ShaderProgram::Location ambientColorLocation = shaderProgram->GetUniformLocation("AmbientColor");
    ShaderProgram::Location lightColorLocation = shaderProgram->GetUniformLocation("LightColor");
    ShaderProgram::Location lightPositionLocation = shaderProgram->GetUniformLocation("LightPosition");
    ShaderProgram::Location cameraPositionLocation = shaderProgram->GetUniformLocation("CameraPosition");
    material->SetShaderSetupFunction([=](ShaderProgram& shaderProgram)
        {
            shaderProgram.SetUniform(worldMatrixLocation, glm::scale(glm::vec3(0.1f)));
            shaderProgram.SetUniform(viewProjMatrixLocation, m_camera.GetViewProjectionMatrix());

            // Set camera and light uniforms
            shaderProgram.SetUniform(ambientColorLocation, m_ambientColor);
            shaderProgram.SetUniform(lightColorLocation, m_lightColor * m_lightIntensity);
            shaderProgram.SetUniform(lightPositionLocation, m_lightPosition);
            shaderProgram.SetUniform(cameraPositionLocation, m_cameraPosition);
        });

    // Configure loader
    ModelLoader loader(material);
    loader.SetCreateMaterials(true);
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Position, "VertexPosition");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Normal, "VertexNormal");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::TexCoord0, "VertexTexCoord");

    // Load model
    m_model = loader.Load("models/mill/Mill.obj");

    // Load and set textures
    Texture2DLoader textureLoader(TextureObject::FormatRGBA, TextureObject::InternalFormatRGBA8);
    textureLoader.SetFlipVertical(true);
    
    //5.1 Load toon ramp texture
    std::vector<std::string> texturePaths =
    {
        "models/mill/Ground_shadow.jpg",
        "models/mill/Ground_color.jpg",
        "models/mill/MillCat_color.jpg"
    };

    for (size_t i = 0; i < m_model.GetMaterialCount(); i++)
    {
        const std::string& texturePath = texturePaths[i];

        auto colorTexture = textureLoader.LoadShared(texturePath.c_str());

        glm::vec3 baseColor = SampleAverageTextureColor(texturePath.c_str());

        glm::vec3 shadowColor = baseColor * 0.35f;
        glm::vec3 litColor = glm::min(baseColor * 1.25f, glm::vec3(1.0f));

        auto toonRamp = CreateToonRampTexture(shadowColor, litColor);

        m_model.GetMaterial(i).SetUniformValue("ColorTexture", colorTexture);
        m_model.GetMaterial(i).SetUniformValue("ToonRamp", toonRamp);
    }

}
    

void ViewerApplication::InitializeCamera()
{
    // Set view matrix, from the camera position looking to the origin
    m_camera.SetViewMatrix(m_cameraPosition, glm::vec3(0.0f));

    // Set perspective matrix
    float aspectRatio = GetMainWindow().GetAspectRatio();
    m_camera.SetPerspectiveProjectionMatrix(1.0f, aspectRatio, 0.1f, 1000.0f);
}

void ViewerApplication::InitializeLights()
{
    // Initialize light variables
    m_ambientColor = glm::vec3(0.25f);
    m_lightColor = glm::vec3(1.0f);
    m_lightIntensity = 1.0f;
    m_lightPosition = glm::vec3(-10.0f, 20.0f, 10.0f);
}

void ViewerApplication::RenderGUI()
{
    m_imGui.BeginFrame();

    ImGui::Text("Toon Shader Controls");
    ImGui::Separator();

    ImGui::Text("Lighting");
    ImGui::DragFloat3("Light position", &m_lightPosition[0], 0.1f);
    /*ImGui::ColorEdit3("Light color", &m_lightColor[0]);
    ImGui::DragFloat("Light intensity", &m_lightIntensity, 0.05f, 0.0f, 10.0f);*/

    ImGui::Separator();

    ImGui::Text("Toon Ramp");
    ImGui::Checkbox("Use material-based ramps", &m_useMaterialColorRamps);
    /*ImGui::DragFloat("Shadow strength", &m_toonShadowStrength, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Highlight strength", &m_toonHighlightStrength, 0.01f, 1.0f, 3.0f);*/

    if (!m_useMaterialColorRamps)
    {
        ImGui::ColorEdit3("Shadow color", &m_toonShadowColor[0]);
        ImGui::ColorEdit3("Lit color", &m_toonLitColor[0]);
    }

    ImGui::Separator();

    ImGui::Text("Debug");
    ImGui::Text("Current mode: %s", m_useMaterialColorRamps ? "Material ramps" : "Custom ramp");
    ImGui::Text("Toon coordinate: max(dot(N, L), 0)");

    if(ImGui::Button("Rebuild Toon Ramps")){
        RebuildToonRamps();
    }

    m_imGui.EndFrame();
}

void ViewerApplication::UpdateCamera()
{
    Window& window = GetMainWindow();

    // Update if camera is enabled (controlled by SPACE key)
    {
        bool enablePressed = window.IsKeyPressed(GLFW_KEY_SPACE);
        if (enablePressed && !m_cameraEnablePressed)
        {
            m_cameraEnabled = !m_cameraEnabled;

            window.SetMouseVisible(!m_cameraEnabled);
            m_mousePosition = window.GetMousePosition(true);
        }
        m_cameraEnablePressed = enablePressed;
    }

    if (!m_cameraEnabled)
        return;

    glm::mat4 viewTransposedMatrix = glm::transpose(m_camera.GetViewMatrix());
    glm::vec3 viewRight = viewTransposedMatrix[0];
    glm::vec3 viewForward = -viewTransposedMatrix[2];

    // Update camera translation
    {
        glm::vec2 inputTranslation(0.0f);

        if (window.IsKeyPressed(GLFW_KEY_A))
            inputTranslation.x = -1.0f;
        else if (window.IsKeyPressed(GLFW_KEY_D))
            inputTranslation.x = 1.0f;

        if (window.IsKeyPressed(GLFW_KEY_W))
            inputTranslation.y = 1.0f;
        else if (window.IsKeyPressed(GLFW_KEY_S))
            inputTranslation.y = -1.0f;

        inputTranslation *= m_cameraTranslationSpeed;
        inputTranslation *= GetDeltaTime();

        // Double speed if SHIFT is pressed
        if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
            inputTranslation *= 2.0f;

        m_cameraPosition += inputTranslation.x * viewRight + inputTranslation.y * viewForward;
    }

    // Update camera rotation
   {
        glm::vec2 mousePosition = window.GetMousePosition(true);
        glm::vec2 deltaMousePosition = mousePosition - m_mousePosition;
        m_mousePosition = mousePosition;

        glm::vec3 inputRotation(-deltaMousePosition.x, deltaMousePosition.y, 0.0f);

        inputRotation *= m_cameraRotationSpeed;

        viewForward = glm::rotate(inputRotation.x, glm::vec3(0,1,0)) * glm::rotate(inputRotation.y, glm::vec3(viewRight)) * glm::vec4(viewForward, 0);
    }

   // Update view matrix
   m_camera.SetViewMatrix(m_cameraPosition, m_cameraPosition + viewForward);
}
