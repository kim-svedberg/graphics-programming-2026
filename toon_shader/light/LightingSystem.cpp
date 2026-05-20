#include "LightingSystem.h"

LightingSystem::LightingSystem(glm::vec3 startingValue){
    m_lightPosition = startingValue;
}

void LightingSystem::InitializeLights(){
    
    // Initialize light variables
    m_lightPosition = glm::vec3(-10.0f, 20.0f, 10.0f);
}

void LightingSystem::SetLightPosition(const glm::vec3& pos){
        m_lightPosition = pos;
}

const glm::vec3& LightingSystem::GetLightPosition() const {

    return m_lightPosition;
}

glm::vec3& LightingSystem::GetLightPosition()
{
    return m_lightPosition;
}