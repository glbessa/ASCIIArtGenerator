#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

// "Ñ@#W$9876543210?!abc;:+=-,._ "
// \xd1 = Ñ
const char *DENSITY = "\xd1@#W$9876543210?!abc;:+=-,._ ";
const int DENSITY_LENGTH = 28;

typedef struct image_t image_t;
struct image_t {
    unsigned char *pixels;
    int width;
    int height;
    int channels;
};

image_t *LoadImage(char *filename, int loadMode);
image_t *ResizeImage(image_t *img, int newWidth, int newHeight);
image_t *CreateImageT(unsigned char *pixels, int width, int height, int channels);
void DestroyImageT(image_t *img);
image_t *ConvertToGray(image_t *img);
image_t *ConvertToASCII(image_t *img);
void PrintASCIIImage(image_t *img);
void SaveASCIIImage(char *filename, image_t *img);

image_t *LoadImage(char *filename, int loadMode) {
    int width, height, channels;
    unsigned char *pixels = stbi_load(filename, &width, &height, &channels, loadMode);
    if (pixels == NULL) {
        perror("Error while opening image");
        exit(-1);
    }

    return CreateImageT(pixels, width, height, channels);
}

image_t *ResizeImage(image_t *img, int newWidth, int newHeight) {
    unsigned char *newPixels = (unsigned char *) malloc(newWidth * newHeight * img->channels * sizeof(unsigned char));
    if (!newPixels) {
        perror("Error while allocating pixels to resized image");
        exit(-1);
    }
    
    stbir_resize_uint8(img->pixels, img->width, img->height, 0, newPixels, newWidth, newHeight, 0, img->channels);

    return CreateImageT(newPixels, newWidth, newHeight, img->channels);
}

image_t *CreateImageT(unsigned char *pixels, int width, int height, int channels) {
    image_t *img = (image_t *) malloc(sizeof(image_t));
    if (!img) {
        perror("Error while allocating image_t");
        exit(-1);
    }

    img->pixels = pixels;
    img->width = width;
    img->height = height;
    img->channels = channels;

    return img;
}

void DestroyImageT(image_t *img) {
    if (!img)
        return;

    if (img->pixels != NULL)
        stbi_image_free(img->pixels);

    free(img);
}

image_t *ConvertToGray(image_t *img) {
    size_t imgSize = img->width * img->height * img->channels;
    int grayChannels = img->channels == 4 ? 2 : 1;
    size_t grayImgSize = img->width * img->height * grayChannels;

    unsigned char *grayPixels = (unsigned char *) malloc(grayImgSize * sizeof(unsigned char));
    if (!grayPixels) {
        perror("Error while allocating gray image");
        exit(-1);
    }

    for (unsigned char *p = img->pixels, *pg = grayPixels; p != img->pixels + imgSize; p += img->channels, pg += grayChannels) {
        *pg = (uint8_t) ((p[0] + p[1] + p[2]) / 3.0);
        
        if (grayChannels == 2) {
            *(pg + 1) = p[3];
        }
    }

    return CreateImageT(grayPixels, img->width, img->height, grayChannels);
}

image_t *ConvertToASCII(image_t *img) {
    size_t asciiImageSize = img->height * img->width;
    unsigned char *asciiPixels = (unsigned char *) malloc(asciiImageSize * sizeof(unsigned char));
    image_t *asciiImage = CreateImageT(asciiPixels, img->width, img->height, 1);

    for (unsigned char *pg = img->pixels, *pascii = asciiPixels; pg != img->pixels + (img->width * img->height * img->channels); pg += img->channels, pascii += 1) {
        *pascii = DENSITY[DENSITY_LENGTH - (uint8_t) (*pg / DENSITY_LENGTH)];

        if (img->channels == 2) {
            *pascii = *pascii * (uint8_t) (*(pg + 1) / 255);
        }
    }

    return asciiImage;
}

void PrintASCIIImage(image_t *img) {
    int i, j;
    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            printf("%c", img->pixels[i * img->width + j]);
        }
        printf("\n");
    }
}

void SaveASCIIImage(char *filename, image_t *img) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error while opening text file to save: ");
        exit(-1);
    }

    int i, j;

    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            fprintf(file, "%c", img->pixels[i * img->width + j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


/*
unsigned char *ConvertToSepia(unsigned char *img, int width, int height, int channels) {

}
*/

#endif