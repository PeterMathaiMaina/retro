#version 450 core
out vec4 FragColor;

in vec2 texCoords;
in vec3 TangentLightDir;
in vec3 TangentViewDir;
in vec3 TangentFragPos;
in vec3 Normal;




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
uniform sampler2D texture_normal1;
uniform sampler2D texture_Displacement1;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirlight;
uniform Spotlight spotlight;
uniform float ScaleHeight;


vec2 ParallaxMapping(vec2 TexCoords,vec3 viewdir){
    float height = texture(texture_Displacement1,TexCoords).r;
    vec2 offset = (viewdir.xy / viewdir.z) * (height * ScaleHeight);
    return TexCoords - offset;
};


    // vec3 Calcdirlight(DirLight light, vec3 normal, vec3 viewDir)
    // {
    //     vec3 lightDir = normalize(-light.direction);
    //     float diff = max(dot(normal, lightDir), 0.0);
    //     vec3 reflectDir = reflect(-lightDir, normal);
    //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    //     vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords).rgb);
    //     vec3 texSpecular = vec3(texture(texture_specular1, TexCoords).rgb);

    //     vec3 ambient = light.ambient * texDiffuse;
    //     vec3 diffuse = light.diffuse * diff * texDiffuse;
    //     vec3 specular = light.specular * spec * texSpecular ;

    //     return ambient + diffuse + specular;
    // }

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

    vec3 texDiffuse = texture(texture_diffuse1, TexCoords).rgb;
    vec3 texSpecular = texture(texture_specular1, TexCoords).rgb;

    vec3 ambient = light.ambient * texDiffuse;
    vec3 diffuse = light.diffuse * diff * texDiffuse * intensity;
    vec3 specular = light.specular * spec * texSpecular * intensity;

    ambient *= attenuation ;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewdir , vec2 TexCoords)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // vec3 viewdir = normalize(viewdir - fragPos );
//     vec3 halfwayDir = normalize(lightDir + viewdir);
    
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

//     // attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + 
//                      light.quadratic * (distance * distance));
//     vec3 texDiffuse = vec3(texture(texture_diffuse1, TexCoords).rgb);
//     vec3 texSpecular = vec3(texture(texture_specular1, TexCoords).rgb);


//     vec3 ambient = light.ambient * texDiffuse;
//     vec3 diffuse = light.diffuse * diff * texDiffuse ;
//     vec3 specular = light.specular * spec * texSpecular ;

//     ambient *= attenuation;
//     diffuse *= attenuation;
//     specular *= attenuation;
//     return (ambient + diffuse + specular);
// }
void main()
{


    vec2 ParallaxCoords = ParallaxMapping(texCoords,TangentViewDir);
    if (ParallaxCoords.x < 0.0 || ParallaxCoords.x > 1.0 || ParallaxCoords.y < 0.0 || ParallaxCoords.y > 1.0)
        discard;



    vec3 norm  = texture(texture_normal1,ParallaxCoords).rgb ;//ParallaxCoords

    norm = normalize(norm * 2.0 - 1.0);   

    vec3 result = vec3(0.0);

    // for (int i = 0; i < NR_POINT_LIGHTS; i++)
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    
    if (spotlight.enabled)
        result += CalcSpotLight(spotlight, norm, TangentLightDir, TangentViewDir, ParallaxCoords);

    // result += Calcdirlight(dirlight, Normal, TangentViewDir); // <- optional: directional light

    // Optional gamma correction
    // result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}
