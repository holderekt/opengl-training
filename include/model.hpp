#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <mesh.hpp>

template <class T>
class Model{
public:
    Model(std::string filename){
        std::ifstream file;
        file.open(filename);
        std::string box;
        std::vector<float> elements;
        std::string value;

        while(!file.eof()){
            getline(file,box);
            std::stringstream stream(box);

            while(getline(stream, value, ',')){
                elements.push_back(std::stof(value));
            }
        }

        vertices = new float[elements.size()];
        v = new Vertex[elements.size() / 8];
        _lenght = elements.size();

        int j = 0;

        for(size_t i=0; i< _lenght; i = i + 8){
            Vertex a;
            a.position = glm::vec3(elements[i],elements[i+1], elements[i +2]);
            a.normal = glm::vec3(elements[i+3], elements[i+4], elements[i+5]);
            a.texture_coordinates = glm::vec2(elements[i+6], elements[i+7]);
            vv.push_back(a);
            j++;
        }
    
        for(size_t i= 0; i!= _lenght; i++){
            vertices[i] = elements[i];
        }
    }

    size_t lenght(){ return _lenght; }
    size_t size(){ return _lenght * sizeof(T); } 
    T* operator()(){ return vertices; }

     Vertex *v;
     std::vector<Vertex> vv;

     std::vector<Vertex> getcose(){return vv;}

private:
    T *vertices;
   
    size_t _lenght;
};
