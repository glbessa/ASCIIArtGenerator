#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb/stb_image_resize.h"

typedef struct image_t image_t;
struct image_t {
    unsigned char *pixels;
    int width;
    int height;
    int channels;
};

typedef enum {
    PNG,
    JPEG,
    BMP
} SAVE_AS;

image_t *image_load(const char *filename, int loadMode);
void image_save(const char *filename, image_t *img, SAVE_AS save_as);
image_t *image_resize(image_t *img, int newWidth, int newHeight);
image_t *image_t_alloc(unsigned char *pixels, int width, int height, int channels);
void image_t_free(image_t *img);

image_t *image_load(const char *filename, int loadMode) {
    int width, height, channels;
    unsigned char *pixels = stbi_load(filename, &width, &height, &channels, loadMode);
    if (pixels == NULL) {
        perror("Error while opening image");
        exit(-1);
    }

    return image_t_alloc(pixels, width, height, channels);
}

void image_save(const char *filename, image_t *img, SAVE_AS save_as) {
    if (!img)
        return;

    switch (save_as) {
        case PNG:
            stbi_write_png(filename, img->width, img->height, img->channels, img->pixels, 0);
            break;
        case JPEG:
            stbi_write_jpg(filename, img->width, img->height, img->channels, img->pixels, 80);
            break;
        case BMP:
            stbi_write_bmp(filename, img->width, img->height, img->channels, img->pixels);
            break;
    }
}

image_t *image_resize(image_t *img, int newWidth, int newHeight) {
    unsigned char *newPixels = (unsigned char *) malloc(newWidth * newHeight * img->channels * sizeof(unsigned char));
    if (!newPixels) {
        perror("Error while allocating pixels to resized image");
        exit(-1);
    }
    
    stbir_resize_uint8(img->pixels, img->width, img->height, 0, newPixels, newWidth, newHeight, 0, img->channels);

    return image_t_alloc(newPixels, newWidth, newHeight, img->channels);
}

image_t *image_t_alloc(unsigned char *pixels, int width, int height, int channels) {
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

void image_t_free(image_t *img) {
    if (!img)
        return;

    if (img->pixels != NULL)
        stbi_image_free(img->pixels);

    free(img);
}


#endif