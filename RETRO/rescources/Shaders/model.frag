#version 330 core
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
//in mat3 TBN;
struct Light
{
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

// Uniforms
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;
uniform vec3 u_ViewPos;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform float u_SpecularStrength;
uniform Light light;

void main()
{
    float distance    = length(u_LightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));  
    // Sample the texture maps
    vec4 diffuseColor = texture(u_DiffuseTexture, TexCoords);
    vec4 specularMapValue = texture(u_SpecularTexture, TexCoords);
    vec3 specularColor = specularMapValue.rgb; // Often specular intensity is in the grayscale

    // --- Ambient Lighting ---
    float ambientStrength = 0.09;
    vec3 ambient = ambientStrength * u_LightColor ; // Ambient influenced by diffuse color*attenuation

    // --- Diffuse Lighting ---
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor * diffuseColor.rgb * attenuation;

    // --- Specular Lighting ---
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess factor
    vec3 specular = u_SpecularStrength * spec * u_LightColor * attenuation;

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, diffuseColor.a); // Use diffuse alpha if available
}