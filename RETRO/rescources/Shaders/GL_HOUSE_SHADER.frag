

#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace; // from vertex shader




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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ao1;
uniform sampler2D texture_emissive1;
uniform samplerCube depthMap;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 u_ViewPos;
uniform DirLight dirLight;
uniform Spotlight spotlight;
uniform float far_plane;
uniform vec3 lightPos;



float PointLightShadow(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    // Sample from cubemap
    float closestDepth = texture(depthMap, fragToLight).r;
    closestDepth *= far_plane; // Convert back to world units

    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords));
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords));

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpecular ;

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

    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords));
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords));

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse * intensity;
    vec3 specular = light.specular * spec * texSpecular * intensity ;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 texDiffuse = texture(texture_diffuse1, TexCoords).rgb;
    vec3 texSpecular = texture(texture_specular1, TexCoords).rgb;

    vec3 ambient = light.ambient * texDiffuse;

    float shadow = PointLightShadow(fragPos); // 0.0 = lit, 1.0 = shadowed
    vec3 diffuse = light.diffuse * diff * texDiffuse * (1.0 - shadow);
    vec3 specular = light.specular * spec * texSpecular * (1.0 - shadow);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 result = vec3(0.0);

    // result +=CalcDirLight(dirLight,Normal,viewDir);


    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    if (spotlight.enabled)
    {
        result += CalcSpotLight(spotlight, norm, FragPos, viewDir);
    }

    // Gamma correction
    // result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
    // FragColor = vec4(1.0);

}

