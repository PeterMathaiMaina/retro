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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_ao1;
uniform sampler2D texture_emissive1;

uniform vec3 u_ViewPos;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;



const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 ComputePBRLighting(vec3 N, vec3 V, vec3 L, vec3 lightColor, vec3 baseColor, float metallic, float roughness)
{
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    // Compute reflectance at normal incidence
    vec3 F0 = mix(vec3(0.04), baseColor, metallic);
    vec3 F = fresnelSchlick(HdotV, F0);

    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * max(NdotV * NdotL, 0.001);
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    vec3 diffuse = kD * baseColor / PI;

    return (diffuse + specular) * lightColor * NdotL;
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

    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords));
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords));

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

    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords));
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords));

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

    vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;
    float metallic = texture(texture_metallic1, TexCoords).r;
    float roughness = texture(texture_roughness1, TexCoords).r;

    vec3 result = vec3(0.0);

    // Directional light
    vec3 dirLightDir = normalize(-dirLight.direction);
    result += ComputePBRLighting(norm, viewDir, dirLightDir, dirLight.diffuse, albedo, metallic, roughness);

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance +
                                   pointLights[i].quadratic * (distance * distance));
        vec3 lightColor = pointLights[i].diffuse * attenuation;
        result += ComputePBRLighting(norm, viewDir, lightDir, lightColor, albedo, metallic, roughness);
    }

    // Spotlight
    if (spotlight.enabled)
    {
        vec3 lightDir = normalize(spotlight.position - FragPos);
        float theta = dot(lightDir, normalize(-spotlight.direction));
        float epsilon = spotlight.cutOff - spotlight.outerCutOff;
        float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(spotlight.position - FragPos);
        float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance +
                                   spotlight.quadratic * (distance * distance));

        vec3 lightColor = spotlight.diffuse * attenuation * intensity;
        result += ComputePBRLighting(norm, viewDir, lightDir, lightColor, albedo, metallic, roughness);
    }

    // Optional ambient occlusion
    float ao = texture(texture_ao1, TexCoords).r;
    result *= ao;

    // Emissive
    vec3 emissive = texture(texture_emissive1, TexCoords).rgb;
    result += emissive;

    // Gamma correction
    result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
    // FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
