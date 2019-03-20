#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <texture.hpp>
#include <shader.hpp>


typedef struct{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texture_coordinates;
}Vertex;


class Mesh{
public:
    std::vector<Vertex> vertexs;
    std::vector<size_t> ebo_indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex>, std::vector<size_t>, std::vector<Texture>);
    void draw(Shader);

private:
    unsigned int VBO, VAO, EBO;

};

Mesh::Mesh(std::vector<Vertex> vertexs, std::vector<size_t> ebo_indices, std::vector<Texture> textures):
vertexs(vertexs), ebo_indices(ebo_indices), textures(textures){

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vertexs.size() * sizeof(Vertex), 
        &vertexs[0], 
        GL_STATIC_DRAW
        );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        ebo_indices.size() * sizeof(size_t), 
        &ebo_indices[0],
        GL_STATIC_DRAW
        );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));
    glEnableVertexAttribArray(2);

    // TODO Bind vao here?? idk
}

void Mesh::draw(Shader shr){
    size_t diffuse_n = 1;
    size_t specular_n = 1;
    std::string varname = "material";

    for(size_t i = 0; i!= textures.size(); i++){
        
        if(textures[i].getType() == TEXTURE_DIFFUSE){
             varname += std::to_string(diffuse_n++) + "diffuse";
        }else if(textures[i].getType() == TEXTURE_SPECULAR){
            varname += std::to_string(specular_n++) + "specular";
        }

        shr.setValue(varname, (int)i);
        textures[i](i);
    }

    glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, ebo_indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertexs.size());
    glBindVertexArray(0);
}