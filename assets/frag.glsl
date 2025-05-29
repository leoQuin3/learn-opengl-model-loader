#version 330 core

#define NR_POINT_LIGHTS 10
#define RENDER_FULLBRIGHT 0

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
    sampler2D texture_specular4;
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;
uniform sampler2D textureSrc;

out vec4 FragColor;

vec3 GetDiffuseTexel();
vec3 GetSpecularTexel();
vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

// TODO:
//      3. Fix Point lighting
//      2. maybe use arrays instead of hardcoding N number of texture_diffuseN's and texture_specularN's, and iterate through each to add onto fragment's color.

void main()
{
#if RENDER_FULLBRIGHT
    FragColor = vec4(GetDiffuseTexel(), 1.0);
#else
    // Calculate vectors
    vec3 normal = normalize(FragNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional light
    //vec3 result = CalcDirLight(dirLight, normal, viewDir);
    vec3 result = GetDiffuseTexel() * 0.1;
    
    // Point lights
    // FIXME: Model goes completely dark after adding to result. Test this function.
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
    
    //// Spot light
    //result += CalcSpotLight(spotLight, normal, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
#endif
}

// Sample diffuse texture
vec3 GetDiffuseTexel()
{
    return vec3(texture(material.texture_diffuse1, TextCoords));
}

// Sample specular texture
vec3 GetSpecularTexel()
{
    return vec3(texture(material.texture_specular1, TextCoords));
}

// Directional lighting
vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = dirLight.ambient * GetDiffuseTexel();
    vec3 diffuse = dirLight.diffuse * diff * GetDiffuseTexel();
    vec3 specular = dirLight.specular * spec * GetSpecularTexel();

    return ambient + diffuse + specular;
}

// Point lighting
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    vec3 ambient = pointLight.ambient * GetDiffuseTexel();
    vec3 diffuse = pointLight.diffuse * diff * GetDiffuseTexel();
    vec3 specular = pointLight.specular * spec * GetSpecularTexel();

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// Spotlight
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = (spotLight.ambient * GetDiffuseTexel());

    vec3 lightDir = normalize(spotLight.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * GetDiffuseTexel();

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * GetSpecularTexel();

    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;

    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}