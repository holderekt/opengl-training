#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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
        _lenght = elements.size();

        for(int i= 0; i!= _lenght; i++){
            vertices[i] = elements[i];
        }
    }

    size_t lenght(){ return _lenght; }
    size_t size(){ return _lenght * sizeof(T); } 
    T* operator()(){ return vertices; }

private:
    T *vertices;
    size_t _lenght;
};
