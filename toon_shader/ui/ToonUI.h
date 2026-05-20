#include <ituGL/camera/Camera.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/utils/DearImGui.h>
#include <ituGL/asset/Texture2DLoader.h>
#include <ituGL/texture/Texture2DObject.h>

struct ToonSettings 
{
    bool useMaterialColorRamps;
    float shadowStrength;
    float highlightStrength;
    glm::vec3 shadowColor;
    glm::vec3 litColor;
    glm::vec3 lightPosition;
       
};

class ToonUI {
private:

public:
    void Render(ToonSettings& settings);
};

