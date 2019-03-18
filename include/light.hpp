#pragma once 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <shader.hpp>



class Light{
public:
    typedef glm::vec3 vec3;

    Light(vec3 ambient, vec3 diffuse, vec3 specular):
        ambient(ambient), diffuse(diffuse), specular(specular){}

    vec3 getAmbient(){return this->ambient;}
    vec3 getDiffuse(){return this->diffuse;}
    vec3 getSpecular(){return this->specular;}

private:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


class DirectionalLight: public Light{
public:
    DirectionalLight(vec3, vec3, vec3, vec3);
    void use(Shader&, const std::string&);

private:
    vec3 direction;
};

DirectionalLight::DirectionalLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction)
    :Light(ambient, diffuse, specular){
        this->direction = direction;
}

void DirectionalLight::use(Shader &shr, const std::string& varname){
    shr.setValue("directionalIsActive", true);
    shr.setValue(varname + ".ambient", this->getAmbient());
    shr.setValue(varname + ".diffuse", this->getDiffuse());
    shr.setValue(varname + ".specular", this->getSpecular());
    shr.setValue(varname + ".direction", this->direction);
}


class PointLight: public Light{
public:
    PointLight(vec3, vec3, vec3, vec3, float, float, float);
    void use(Shader&, const std::string&, int);
    void setPosition(vec3 position){ this->position = position; }

private:
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

PointLight::PointLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, 
    float constant, float linear, float quadratic):Light(ambient, diffuse, specular){
        this->position = position;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
}

void PointLight::use(Shader &shr, const std::string& varname, int position){
        std::string pos = std::to_string(position);
        shr.setValue(varname + "[" + pos + "]" + ".ambient", this->getAmbient());
        shr.setValue(varname + "[" + pos + "]" + ".diffuse", this->getDiffuse());
        shr.setValue(varname + "[" + pos + "]" + ".specular", this->getSpecular());
        shr.setValue(varname + "[" + pos + "]" + ".position", this->position);
        shr.setValue(varname + "[" + pos + "]" + ".constant", this->constant);
        shr.setValue(varname + "[" + pos + "]" + ".quadratic", this->linear);
        shr.setValue(varname + "[" + pos + "]" + ".linear", this->quadratic);
}





