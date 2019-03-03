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
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

void main(){
    
   
    vec3 ambient = light.ambient * texture(material.diffuse , Tex).rgb;
  	
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse , Tex).rgb;

    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), material.shinnes);
    vec3 specular = light.specular * spec * texture(material.specular, Tex).rgb; 
            
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
  

}