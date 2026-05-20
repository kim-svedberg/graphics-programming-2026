set(TARGETNAME toon_shader)

set(libraries glad glfw assimp imgui itugl ${APPLE_LIBRARIES})

file(GLOB_RECURSE target_inc "*.h")
file(GLOB_RECURSE target_src "*.cpp")

file(GLOB_RECURSE shaders "*.vert" "*.frag" "*.geom" "*.glsl")
source_group("Shaders" FILES ${shaders})

add_executable(${TARGETNAME} ${target_inc} ${target_src} ${shaders}
    ui/ToonUI.cpp
    settings/SettingsManager.cpp
    render/ModelRenderer.cpp
    light/LightingSystem.cpp
    camera/ToonCamera.cpp
    util/TextureUtils.cpp)
target_link_libraries(${TARGETNAME} ${libraries})