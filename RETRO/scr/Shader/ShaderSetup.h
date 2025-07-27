#ifndef SHADERSETUP
#define SHADERSETUP
#include <GL/glew.h>
#include "../core/Shader.hpp"
#include "../../third_party/glm/glm.hpp"
#include "../../third_party/glm/gtc/matrix_transform.hpp"
#include "../../third_party/glm/gtc/type_ptr.hpp"

void setDirLight(Shader& shader, const glm::vec3& direction,const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
void setPointLight(Shader& shader, const std::string& name, const glm::vec3& color,const glm::vec3& position,const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,float constant, float linear, float quadratic);
void setSpotLight(Shader& shader, const std::string& name, const glm::vec3& position,const glm::vec3& direction, const glm::vec3& ambient,const glm::vec3& diffuse, const glm::vec3& specular,float constant, float linear, float quadratic,float cutOff, float outerCutOff, bool enabled);
void setMatrices( const Shader& shader, const glm::mat4& model,const glm::mat4& Projection,const glm::mat4& view);

#endif