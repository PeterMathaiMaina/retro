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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ao1;
uniform sampler2D texture_emissive1;
uniform sampler2D shadowMap;


uniform vec3 u_ViewPos;
uniform DirLight dirLight;
// uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;

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

// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance +
//                                light.quadratic * (distance * distance));

//     vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords));
//     vec3 texSpecular = vec3(texture(texture_specular1, TexCoords));

//     vec3 ambient = light.ambient * texDiffuse;
//     vec3 diffuse = light.diffuse * diff * texDiffuse;
//     vec3 specular = light.specular * spec * texSpecular * u_SpecularStrength;

//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;

//     return ambient + diffuse + specular;
// }

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



float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    if (projCoords.z > 1.0)
        return 0.0;

    float shadow = 0.0;
    float bias = 0.005;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}



void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 result = vec3(0.0);

    // Directional light (split ambient and rest)
    vec3 texDiffuse = texture(texture_diffuse1, TexCoords).rgb;

    vec3 ambient = dirLight.ambient * texDiffuse;
    vec3 dirLightNoAmbient = CalcDirLight(dirLight, norm, viewDir) - ambient;

    float shadow = ShadowCalculation(FragPosLightSpace, shadowMap);
    vec3 shadowedDirLight = ambient + (1.0 - shadow) * dirLightNoAmbient;

    result += shadowedDirLight;

    // Spotlight (not affected by directional shadow map)
    if (spotlight.enabled)
    {
        result += CalcSpotLight(spotlight, norm, FragPos, viewDir);
    }

    // Ambient Occlusion (optional)
    // float ao = texture(texture_ao1, TexCoords).r;
    // result *= ao;

    // Emissive (optional)
    // vec3 emissive = texture(texture_emissive1, TexCoords).rgb;
    // result += emissive;

    // Gamma correction
    // result = pow(result, vec3(1.0 / 2.2));
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
    // FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
    // FragColor = vec4(texture(shadowMap,TexCoords).r);
    // debug.frag

}

