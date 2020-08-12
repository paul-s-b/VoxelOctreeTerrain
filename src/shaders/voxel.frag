#version 410 core

in vec3 normal_v; // normals
in vec3 dir_v;    // view direction
in float dist_v;  // distance from camera
in vec3 FragPos;  // fragment position
in vec3 color_v;  // vertex color
in vec3 campos_v; // camera position

out vec3 fragColor;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 lightColor;
};

struct PointLight {
    vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 lightColor;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float distance);

void	main()
{
    DirLight dLight;
    PointLight pLight;

    pLight.position = vec3(campos_v);
    pLight.ambient = vec3(0.4f, 0.4f, 0.4f);
    pLight.diffuse = vec3(0.4f, 0.4f, 0.4f);
    pLight.specular = vec3(1.0f, 1.0f, 1.0f);
    pLight.constant = 1.0;
    pLight.linear = 0.09;
    pLight.quadratic = 0.032;
    pLight.lightColor = vec3(1.0f, 1.0f, 1.0f);
          
    dLight.direction = vec3(0.f, -1.f, 0.f);
    dLight.ambient = vec3(0.8f, 0.8f, 0.8f);
    dLight.diffuse = vec3(0.4f, 0.4f, 0.4f);
    dLight.specular = vec3(1.0f, 1.0f, 1.0f);
    dLight.lightColor = vec3(1.0f, 1.0f, 1.0f);

	vec3 norm = normalize(normal_v);
    vec3 result = CalcDirLight(dLight, norm, dir_v);

    //for(int i = 0; i < 1; i++)
    //    result += CalcPointLight(pLight, norm, FragPos, dir_v, dist_v);  

	fragColor = result * color_v;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient * light.lightColor;
    vec3 diffuse = light.diffuse * diff * light.lightColor;
    vec3 specular = light.specular * spec * light.lightColor;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float distance)
{
    vec3 lightDir = normalize(fragPos - light.position);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * light.lightColor;
    vec3 diffuse = light.diffuse * diff * light.lightColor;
    vec3 specular = light.specular * spec * light.lightColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}