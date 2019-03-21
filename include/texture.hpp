#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <png.h>
#include <cstdint>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


typedef enum {
    TEXTURE_SPECULAR,
    TEXTURE_DIFFUSE
}TEXTURE_TYPE;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}__attribute__((packed))RGB8Pixel;


typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
}__attribute__((packed))RGBA8Pixel;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t gray;
}__attribute__((packed))GRAYSCALE8Pixel;


class Texture{
public:
    Texture(){ glGenTextures(1, &ID);}
    Texture(std::string, TEXTURE_TYPE);
    void load_image(std::string);
    void use();
    void use(unsigned int);
    TEXTURE_TYPE getType();

private:
    unsigned int ID;
    TEXTURE_TYPE type;

    template<class T>
    void _load_bytes(png_structp png_ptr,uint32_t width, uint32_t height, T* image, int);
};


TEXTURE_TYPE Texture::getType(){
    return type;
}

void Texture::use(){
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::use(unsigned int value){
    glActiveTexture(GL_TEXTURE0 + value);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::Texture(std::string filename, TEXTURE_TYPE type){
    this->type = type;
    glGenTextures(1, &ID);
    load_image(filename);
}

void Texture::load_image(std::string filename){

    uint32_t width;
    uint32_t height;
    int bit_depth, color_type;

    FILE *file;

    if((file = fopen(filename.c_str(), "rb")) == nullptr){
        std::cout << "Errore" << std::endl;
        throw "Errore";
    }

    unsigned char sig[8];

    fread(sig, 1,8, file);
    if(!png_check_sig(sig,8)){
        std::cout << "Errore" << std::endl;
        throw "Errore";
    }

    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
      &color_type, NULL, NULL, NULL);


    if(bit_depth == 16)
        png_set_strip_16(png_ptr);

    int lenght = width * height;
    
    if(color_type == PNG_COLOR_TYPE_RGB){
         _load_bytes(png_ptr, width, height, new RGB8Pixel[lenght], GL_RGB);
    }else if(color_type == PNG_COLOR_TYPE_RGBA){
         _load_bytes(png_ptr, width, height, new RGBA8Pixel[lenght], GL_RGBA);
    }else if (color_type == PNG_COLOR_TYPE_GRAY){
         _load_bytes(png_ptr, width, height, new GRAYSCALE8Pixel[lenght], GL_LUMINANCE8);
    }
}

template <class T>
void Texture::_load_bytes(png_structp png_ptr,uint32_t width, uint32_t height, T* image, int rgb_type){

    T**colors = new T*[height];

    for(int i =0; i!= (int)height; i++){
        colors[i] = new T[width];
    }

    png_read_image(png_ptr, (png_bytepp)(colors));

    for(int i =0; i!= (int)height; i++){
        for(int j =0; j!= (int)width; j++){
            image[i*width + j] = colors[i][j];
        }
    }

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, rgb_type, width, height, 0, rgb_type, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
}