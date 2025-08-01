#version 450 core
out vec4 FragColor;

in vec2 texCoords;
in vec3 TangentLightDir;
in vec3 TangentViewDir;
in vec3 TangentFragPos;
in vec3 fragPos;

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
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

    bool SpecularEnabled;
    bool enabled;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform Spotlight spotlight;
uniform PointLight pointlight;


vec3 CalcSpotLight(Spotlight light, vec3 normal, vec3 lightDir, vec3 viewDir, vec2 TexCoords)
{
    
    lightDir = normalize(lightDir);
    viewDir = normalize(viewDir);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);


    float theta = dot(lightDir, normalize(-light.direction)); // corrected
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - TangentFragPos); // corrected
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 texDiffuse = texture(texture_diffuse1, texCoords).rgb;
    vec3 texSpecular = texture(texture_specular1, texCoords).rgb;

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse * intensity;
    vec3 specular = light.specular * spec * texSpecular * intensity;

    ambient *= attenuation ;
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
    

    vec3 texDiffuse = texture(texture_diffuse1, texCoords).rgb;
    vec3 texSpecular = texture(texture_specular1, texCoords).rgb;
    // vec3 lightcolor = light.color * (attenuation /5);


    vec3 ambient = light.ambient * texDiffuse;// * lightcolor;

    vec3 diffuse = light.diffuse * diff * texDiffuse ;
    vec3 specular = light.specular * spec * texSpecular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
};
void main()
{
    vec3 norm = texture(texture_normal1, texCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 result = vec3(0.0);
    result += CalcPointLight(pointlight, norm, fragPos, TangentLightDir);

    if (spotlight.enabled)
        result += CalcSpotLight(spotlight, norm, TangentLightDir, TangentViewDir, texCoords);
    FragColor = vec4(result, 1.0);
}