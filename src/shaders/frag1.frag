#version 420 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 Tex;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shinnes;
};

struct Light{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};



uniform Light light;
uniform Material material;
uniform PointLight pLight;
uniform DirectionalLight dLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shinnes);

    vec3 ambient = light.ambient * texture(material.diffuse , Tex).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse , Tex).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, Tex).rgb; 

    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){


    float distances = length(light.position - fragPos);
    float attenuation = (1.0/(light.constant + light.linear * distances + light.quadratic * (distances * distances)));
    
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shinnes);
 
    vec3 ambient = light.ambient * texture(material.diffuse , Tex).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse , Tex).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, Tex).rgb; 


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);

}

void main(){

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 lightCalculations = calculatePointLight(pLight, norm, FragPos, viewDir);
            
    vec3 result = lightCalculations * (objectColor * lightColor);
    FragColor = vec4(result, 1.0);
  

}

