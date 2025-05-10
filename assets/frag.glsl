#version 330 core

in vec3 FragPos;
in vec3 FragNorm;
in vec2 TextCoords;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
#define RENDER_FULLBRIGHT 1

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform sampler2D textureSrc;

out vec4 FragColor;

vec3 GetDiffuseColor();
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


//TODO: maybe use arrays instead of hardcoding N number of texture_diffuseN's and texture_specularN's, and iterate through each to add onto fragment's color.

void main()
{
#if RENDER_FULLBRIGHT
    FragColor = vec4(GetDiffuseColor(), 1.0);
#else
    // Calculate vectors
    vec3 normal = normalize(FragNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional light
    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
    
    // Spot light
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
#endif
}

vec3 GetDiffuseColor()
{
    vec3 diffuse = vec3(texture(material.texture_diffuse1, TextCoords));
    return diffuse;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // Calculate vectors and specular strength
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), /*material.shininess*/ 1);

    // Lighting maps
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TextCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TextCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TextCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Calculate vectors and specular strength
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Lighting maps
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TextCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TextCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TextCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = (light.ambient * vec3(texture(material.texture_diffuse1, TextCoords)));
    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TextCoords));

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TextCoords));

    // Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;

    // Cutoff
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // Result
    return (ambient + diffuse + specular);
}