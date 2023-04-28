#ifndef IMAGE_ASCII_H
#define IMAGE_ASCII_H

#include "utils.h"

image_t *ascii_image_load(const char *filename);
void ascii_image_print(image_t *img);
void ascii_image_save(const char *filename, image_t *img);

image_t *ascii_image_load(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error while opening ascii image");
        exit(-1);
    }
    
    int actualChar;
    size_t width = 0;
    size_t height = 0;

    
    while ((actualChar = fgetc(fp)) != EOF) {
        if (actualChar == '\n') {
            height++;
            continue;
        }

        width++;
    }
    height++;
    

    fseek(fp, 0, SEEK_SET);
    
    unsigned char *asciiPixels = (unsigned char *) malloc(sizeof(unsigned char) * height * width);

    int i = 0;
    while ((actualChar = fgetc(fp)) != EOF) {
        asciiPixels[i] = (unsigned char) actualChar;
        i++;
    }

    if (fp)
        fclose(fp);

    return image_t_alloc(asciiPixels, width, height, 1);
}

void ascii_image_print(image_t *img) {
    int i, j;
    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            printf("%c", img->pixels[i * img->width + j]);
        }
        printf("\n");
    }
}

void ascii_image_save(const char *filename, image_t *img) {
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

#endif