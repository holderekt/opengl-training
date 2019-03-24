#pragma once 

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <shader.hpp>
#include <mesh.hpp>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstring>

class Model2{
public:
    Model2(std::string path){load_model(path);}
    void draw(Shader);

    void load_model(std::string);
    void process_node(aiNode*, const aiScene*);
    Mesh process_mesh(aiMesh*, const aiScene*);
    std::vector<Texture> load_material_texture(aiMaterial*, aiTextureType, TEXTURE_TYPE);

private:
    std::vector<Mesh> meshes;
    std::string directory;

};

void Model2::draw(Shader shr){
    for(size_t i=0; i!=meshes.size(); i++){
        meshes[i].draw(shr);
    }     
}

void Model2::load_model(std::string path){
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);

}

void Model2::process_node(aiNode* node, const aiScene* scene){
    for(unsigned int i=0; i< node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for(unsigned int i=0; i< node->mNumChildren; i++){
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model2::process_mesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> vertexs;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i=0; i < mesh->mNumVertices; i++){
        Vertex vertex;
        glm::vec3 gVertex;

        gVertex.x = mesh->mVertices[i].x;
        gVertex.y = mesh->mVertices[i].y;
        gVertex.z = mesh->mVertices[i].z;
        vertex.position = gVertex;


        gVertex.x = mesh->mNormals[i].x;
        gVertex.y = mesh->mNormals[i].y;
        gVertex.z = mesh->mNormals[i].z;
        vertex.normal = gVertex;

        if(mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coordinates = vec;
        }else{
            vertex.texture_coordinates = glm::vec2(0.0f,0.0f);
        }

        vertexs.push_back(vertex);
    }

    for(unsigned int i=0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse = load_material_texture(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE);
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        std::vector<Texture> specular = load_material_texture(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR);
        textures.insert(textures.end(), specular.begin(), specular.end());
    }

    return Mesh(vertexs, indices, textures);

}

std::vector<Texture> Model2::load_material_texture(aiMaterial* material, aiTextureType aitype, TEXTURE_TYPE type){
    std::vector<Texture> textures;
    for(unsigned int i=0; i < material->GetTextureCount(aitype); i++){
        aiString str;
        material->GetTexture(aitype, i, &str);
        std::cout << directory + "/" + str.C_Str() << std::endl;
        Texture tex(directory + "/" + str.C_Str(), type);
        textures.push_back(tex);
    }

    return textures;
}