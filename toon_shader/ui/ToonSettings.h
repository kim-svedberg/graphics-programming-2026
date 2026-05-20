#pragma once

#include <glm/glm.hpp>

struct ToonSettings 
{
    bool useMaterialColorRamps;
    float shadowStrength;
    float highlightStrength;
    glm::vec3 shadowColor;
    glm::vec3 litColor;
    glm::vec3 lightPosition;
       
};