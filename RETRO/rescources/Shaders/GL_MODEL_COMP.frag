#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    bool enabled;
};

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;
uniform sampler2D u_MetallicTexture;
uniform vec3 u_ViewPos;
uniform float u_SpecularStrength;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 texDiffuse = vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 texSpecular = vec3(texture(u_SpecularTexture, TexCoords));

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpecular * u_SpecularStrength;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 texDiffuse = vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 texSpecular = vec3(texture(u_SpecularTexture, TexCoords));

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpecular * u_SpecularStrength;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 texDiffuse = vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 texSpecular = vec3(texture(u_SpecularTexture, TexCoords));

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse * intensity;
    vec3 specular = light.specular * spec * texSpecular * intensity * u_SpecularStrength;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    if (spotlight.enabled)
        result += CalcSpotLight(spotlight, norm, FragPos, viewDir);
    
    //float metallic = texture(u_MetallicTexture, TexCoords).r;
    //result = metallic *result;
    result = clamp(result, 0.0, 1.0); // avoid overflow or underflow



    FragColor = vec4(result, 1.0);

    // Debug fallback:
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0); // red
}
