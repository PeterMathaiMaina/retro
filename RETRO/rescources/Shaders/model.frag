#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1

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
uniform vec3 u_ViewPos;
uniform float u_SpecularStrength;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;

float near = 0.5;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5);

    vec3 ambient  = light.ambient  * vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_SpecularTexture, TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_SpecularTexture, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(Spotlight spotlight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(spotlight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.6);

    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(spotlight.position - fragPos);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + 
                               spotlight.quadratic * (distance * distance));

    vec3 ambient  = spotlight.ambient  * vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 diffuse  = spotlight.diffuse  * diff * vec3(texture(u_DiffuseTexture, TexCoords));
    vec3 specular = spotlight.specular * spec * vec3(texture(u_SpecularTexture, TexCoords));

    diffuse *= intensity;
    specular *= intensity;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    if (spotlight.enabled)
        result += CalcSpotLight(spotlight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
    // Debug color
    //FragColor = vec4(0.5,0.0,0.0, 1.0);
}
