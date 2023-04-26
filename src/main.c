#include "image_functions.h"

#include <stdio.h>
#include <stdlib.h>

void PrintHelp();

int main(int argc, char **argv)
{
    if (argc == 1) {
        PrintHelp();
        exit(0);
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];
    int resize_width = 0, resize_height = 0;

    image_t *image = LoadImage(input_filename, 0);

    if (argc == 5) {
        resize_width = atoi(argv[3]);
        resize_height = atoi(argv[4]);
    } else {
        resize_width = image->width;
        resize_height = image->height;
    }

    // First you need to convert to grayscale
    image_t *gray = ConvertToGray(image);
    // Resize the image if you want
    image_t *resized = ResizeImage(gray, resize_width, resize_height);
    // Now we can convert to ascii
    image_t *ascii = ConvertToASCII(resized);

    //PrintASCIIImage(ascii);
    SaveASCIIImage(output_filename, ascii);

    //stbi_write_jpg("gray.jpeg", width, height, gray_channels, gray_img, 80);

    DestroyImageT(image);
    DestroyImageT(gray);
    DestroyImageT(resized);
    DestroyImageT(ascii);

    return 0;
}

void PrintHelp() {
    printf("Help message\n");
}