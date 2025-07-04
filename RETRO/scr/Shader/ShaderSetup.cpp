#include "ShaderSetup.h"
void setDirLight(Shader& shader, const glm::vec3& direction,
                 const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) {
    shader.setvec3("dirLight.direction", direction);
    shader.setvec3("dirLight.ambient", ambient);
    shader.setvec3("dirLight.diffuse", diffuse);
    shader.setvec3("dirLight.specular", specular);
}
void setPointLight(Shader& shader, const std::string& name, const glm::vec3& position,
                   const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                   float constant, float linear, float quadratic) {
    shader.setvec3(name + ".position", position);
    shader.setvec3(name + ".ambient", ambient);
    shader.setvec3(name + ".diffuse", diffuse);
    shader.setvec3(name + ".specular", specular);
    shader.setFloat(name + ".constant", constant);
    shader.setFloat(name + ".linear", linear);
    shader.setFloat(name + ".quadratic", quadratic);
}
void setSpotLight(Shader& shader, const std::string& name, const glm::vec3& position,
                  const glm::vec3& direction, const glm::vec3& ambient,
                  const glm::vec3& diffuse, const glm::vec3& specular,
                  float constant, float linear, float quadratic,
                  float cutOff, float outerCutOff, bool enabled) {
    shader.setvec3(name + ".position", position);
    shader.setvec3(name + ".direction", direction);
    shader.setvec3(name + ".ambient", ambient);
    shader.setvec3(name + ".diffuse", diffuse);
    shader.setvec3(name + ".specular", specular);
    shader.setFloat(name + ".constant", constant);
    shader.setFloat(name + ".linear", linear);
    shader.setFloat(name + ".quadratic", quadratic);
    shader.setFloat(name + ".cutOff", cutOff);
    shader.setFloat(name + ".outerCutOff", outerCutOff);
    shader.setBool(name + ".enabled", enabled);
}
