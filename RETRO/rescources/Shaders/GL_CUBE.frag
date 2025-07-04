#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;



struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool SpecularEnabled;
};
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

    bool SpecularEnabled;
    bool enabled;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D shadowMap;
uniform vec3 u_ViewPos;
uniform float u_SpecularStrength;
uniform bool SpecularEnabled;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirlight;
uniform Spotlight spotlight;

vec3 Calcdirlight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords).rgb);
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords).rgb);

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse;
    vec3 specular = light.specular * spec * texSpecular * u_SpecularStrength;

    if (SpecularEnabled)
        return specular;
    else
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

    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords).rgb);
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords).rgb);

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse * intensity;
    vec3 specular = light.specular * spec * texSpecular * intensity * u_SpecularStrength;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    if(SpecularEnabled)
        return vec3(texture(texture_specular1, TexCoords).rgb);
    return ambient + diffuse + specular;
    
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // vec3 viewDir = normalize(viewDir - fragPos );
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                     light.quadratic * (distance * distance));
    vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords).rgb);
    vec3 texSpecular = vec3(texture(texture_specular1, TexCoords).rgb);


    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse ;
    vec3 specular = light.specular * spec * texSpecular ;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    if(SpecularEnabled)
        return  vec3(texture(texture_specular1, TexCoords).rgb);

    return (ambient + diffuse + specular);
}
float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;

    float shadow = 0.0;
    float bias = max(0.005 * (1.0 - dot(normalize(Normal), normalize(dirlight.direction))), 0.0005);
    float samples = 4.0;
    float offset = 1.0 / 1024.0; // assuming 1024x1024 shadow map size

    for(float x = -1.5; x <= 1.5; x += 1.0)
    {
        for(float y = -1.5; y <= 1.5; y += 1.0)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * offset).r;
            shadow += (projCoords.z - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= (samples * samples);

    return 0.0;
}
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 dirLightResult = Calcdirlight(dirlight, norm, viewDir);
    float shadow = ShadowCalculation(FragPosLightSpace);
    dirLightResult *= (1.0 - shadow);

    vec3 result = dirLightResult;

    // for (int i = 0; i < NR_POINT_LIGHTS; i++)
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    if (spotlight.enabled)
        result += CalcSpotLight(spotlight, norm, FragPos, viewDir);
    // Optional gamma correction
    // result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}
