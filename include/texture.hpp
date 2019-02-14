#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <png.h>
#include <cstdint>

class Texture{
public:
    Texture(char*);

};


/*
    now with the right paddind it should work
*/
struct pixel{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}__attribute__((packed));


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

        	for (y=0; y < height; y++) {
		png_byte* row = row_pointers[y];
		for (x=0; x<width; x++) {
			if (color_type == PNG_COLOR_TYPE_RGB) {
				png_byte *ptr = &(row[x*3]);
				printf("Pixel at position (%d,%d) has RGBA values (%d,%d,%d,%d)", x ,y, ptr[0], ptr[1], ptr[2], 0);
			}
			if (color_type == PNG_COLOR_TYPE_RGBA) {
				png_byte *ptr = &(row[x*4]);
				printf("Pixel at position (%d,%d) has RGBA values (%d,%d,%d,%d)", x, y, ptr[0], ptr[1], ptr[2], ptr[3]);
			}
			printf("\n");
            int a;
            std::cin >> a;
		}
	}

        fclose(fp);
    
    std::cout << width << std::endl;
    std::cout << height << std::endl;

}   