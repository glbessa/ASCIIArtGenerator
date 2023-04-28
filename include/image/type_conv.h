#ifndef IMAGE_TYPE_CONV_H
#define IMAGE_TYPE_CONV_H

#include "ascii.h"
#include "utils.h"

#include "../string_extra.h"

#include <dirent.h>

typedef enum {
    GRAY,
    ASCII,
    SEPIA
} TYPE;

image_t *convert_to_gray(image_t *img);
image_t *convert_to_ascii(image_t *img);
void convert_file(const char *filename, const char *saveFilename, int width, int height, TYPE type);

// "Ñ@#W$9876543210?!abc;:+=-,._ "
// \xd1 = Ñ
const char *DENSITY = "\xd1@#W%$9876543210?!qljaboci;:+=-,._¨ ";
const int DENSITY_LENGTH = 36;

image_t *convert_to_gray(image_t *img) {
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

    return image_t_alloc(grayPixels, img->width, img->height, grayChannels);
}

image_t *convert_to_ascii(image_t *img) {
    size_t asciiImageSize = img->height * img->width;
    unsigned char *asciiPixels = (unsigned char *) malloc(asciiImageSize * sizeof(unsigned char));
    image_t *asciiImage = image_t_alloc(asciiPixels, img->width, img->height, 1);

    for (unsigned char *pg = img->pixels, *pascii = asciiPixels; pg != img->pixels + (img->width * img->height * img->channels); pg += img->channels, pascii += 1) {
        *pascii = DENSITY[DENSITY_LENGTH - (uint8_t) (*pg / DENSITY_LENGTH)];

        if (img->channels == 2) {
            *pascii = *pascii * (uint8_t) (*(pg + 1) / 255);
        }
    }

    return asciiImage;
}

/*
unsigned char *convert_to_sepia(unsigned char *img, int width, int height, int channels) {

}
*/

void convert_file(const char *filename, const char *saveFilename, int width, int height, TYPE type_conv) {
    image_t *image = image_load(filename, 0),*resized, *final_img;

    if (width != 0 && height != 0) {
        resized = image_resize(image, width, height);
        image_t_free(image);
    } else {
        resized = image;
    }

    switch (type_conv) {
        case GRAY:
            final_img = convert_to_gray(resized);
            image_save(saveFilename, final_img, PNG);
            break;
        case ASCII:
            image_t *gray = convert_to_gray(resized);
            final_img = convert_to_ascii(gray);
            ascii_image_save(saveFilename, final_img);
            image_t_free(gray);
            break;
        case SEPIA:
            break;
    }

    image_t_free(final_img);
    image_t_free(resized);

    if (!image)
        image_t_free(image);
}

void convert_directory(const char *path, const char *savePath, int width, int height, TYPE type_conv) {
    struct dirent *de;
    char *filename, *saveFilename, *fullFilename;
    size_t saveFilenameLength = strlen(savePath);
    
    //linked_list_t files;

    DIR *dr = opendir(path);
    if (!dr) {
        perror("Error while opening directory");
        exit(-1);
    }

    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;

        filename = de->d_name;
        printf("%s\n", filename);
        saveFilenameLength += strlen(filename) + 2;
        saveFilename = (char *) malloc(sizeof(char) * saveFilenameLength);
        fullFilename = (char *) malloc(sizeof(char) * (strlen(filename) + strlen(path) + 2));
        if (!saveFilename) {
            perror("Error while allocating save filename");
            exit(-1);
        }

        strcpy(saveFilename, savePath);
        strcat(saveFilename, "/");
        strcat(saveFilename, filename);
        str_replace(saveFilename, "png", "txt");

        strcpy(fullFilename, path);
        strcat(fullFilename, "/");
        strcat(fullFilename, filename);

        printf("%s\n", saveFilename);

        convert_file(fullFilename, saveFilename, width, height, type_conv);
    }

    closedir(dr);
}

#endif