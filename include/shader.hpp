#pragma once

#include <glad/glad.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader{
    public:
        unsigned int ID;

        Shader(const GLchar*, const GLchar*);
        void operator()();
        void setValue(const std::string&, float);

        // TODO
        void setValue(const std::string&, bool);
        void setValue(const std::string&, int);
    private:
        std::string _loadShader(const GLchar*);
};

Shader::Shader(const GLchar* vertex_file, const GLchar* fragment_file){

    std::string vC = _loadShader(vertex_file);
    std::string fC = _loadShader(fragment_file);
    const char* vertexCode = vC.c_str();
    const char* fragmentCode = fC.c_str();

    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::_loadShader(const GLchar* shader_file){
    std::ifstream file_stream;
    file_stream.open(shader_file);
    std::stringstream stringStream;

    stringStream << file_stream.rdbuf();
    file_stream.close();

    return stringStream.str();
}

void Shader::operator()(){
    glUseProgram(ID);
}

void Shader::setValue(const std::string& name, float value){
    int uniformLocation = glGetUniformLocation(ID, name.c_str());
    glUniform1f(uniformLocation, value);
}
