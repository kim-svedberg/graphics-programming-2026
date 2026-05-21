#pragma once

#include <glm/glm.hpp>

struct ToonSettings 
{
    bool useDefaultColorRamps;
    float outlineThickness;
    glm::vec3 outlineColor;
    float shadowStrength;
    float highlightStrength;
    glm::vec3 shadowColor;
    glm::vec3 litColor;
    glm::vec3 lightPosition;
       
};