#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <png.h>
#include <cstdint>

class Texture{
public:
    Texture(char*);

    void load_cose(png_bytepp inizio, png_bytepp fine, int height, int width){
        for(int i = 0; i!=height; i++)
            for(int j =0; j!=width; j++)
                fine[i][j] = inizio[i][j];
    }

};


/*
    now with the right paddind it should work
*/


struct RGBPixel{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


Texture::Texture(char* filename){
        
    int x, y;

    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    

   char header[8];    // 8 is the maximum size that can be checked

        /* open file and test for it being a png */
        FILE *fp = fopen(filename, "rb");

        fread(header, 1, 8, fp);


        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    
        info_ptr = png_create_info_struct(png_ptr);

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);


       png_bytepp row_pointers;



        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (y=0; y<height; y++)
                
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

        png_read_image(png_ptr, row_pointers);

        RGBPixel **color = new RGBPixel*[height];
        for(int i = 0; i!=height; i++)
            color[i] = new RGBPixel[width];

        load_cose(row_pointers, (png_bytepp)color, height, width);

        RGBPixel *result = new RGBPixel[width * height];

        for(int i = 0; i!=height; i++)
            for(int j =0; j!=width; j++)
                result[i*height + j] = color[i][j];
      
        // for(int i = 0; i!=(height*width); i++)
        //     printf("%d %d %d\n", result[i].r, result[i].g, result[i].b);
        


        fclose(fp);
    
    std::cout << width << std::endl;
    std::cout << height << std::endl;

}   

