#ifndef COLOR_TERM_H
#define COLOR_TERM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\x1b[0m"

#define FG_RGB "\x1b[38;2;%hi;%hi;%him"
#define BG_RGB "\x1b[48;2;%hi;%hi;%him"

typedef struct color_rgb_t color_rgb_t;
struct color_rgb_t {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

char *get_ansi_rgb_foreground(unsigned char red, unsigned char green, unsigned char blue);
char *get_ansi_rgb_background(unsigned char red, unsigned char green, unsigned char blue);
void print_rgb(const char *str, unsigned char fr, unsigned char fg, unsigned char fb, unsigned char br, unsigned char bg, unsigned char bb);
void println_rgb(const char *str, unsigned char fr, unsigned char fg, unsigned char fb, unsigned char br, unsigned char bg, unsigned char bb);

char *get_ansi_rgb_foreground(unsigned char red, unsigned char green, unsigned char blue) {
    char *colorSpec = (char *) malloc(sizeof(char) * 19);
    if (!colorSpec) {
        perror("Error while allocating colorSpec");
        exit(-1);
    }

    sprintf(colorSpec, FG_RGB, red, green, blue);

    return colorSpec;
}

char *get_ansi_rgb_background(unsigned char red, unsigned char green, unsigned char blue) {
    char *colorSpec = (char *) malloc(sizeof(char) * 19);
    if (!colorSpec) {
        perror("Error while allocating colorSpec");
        exit(-1);
    }

    sprintf(colorSpec, BG_RGB, red, green, blue);

    return colorSpec;
}

void print_rgb(const char *str, unsigned char fr, unsigned char fg, unsigned char fb, unsigned char br, unsigned char bg, unsigned char bb) {
    if (strcmp(getenv("COLORTERM"), "truecolor") != 0)
        return;

    if (fr != -1 && fg != -1 && fb != -1) {
        char *foreColorSpec = (char *) malloc(sizeof(char) * 19);
        if (!foreColorSpec) {
            perror("Error while allocating colorSpec");
            exit(-1);
        }
        sprintf(foreColorSpec, FG_RGB, fr, fg, fb);
        printf("%s", foreColorSpec);
        free(foreColorSpec);
    }
    if (br != -1 && bg != -1 && bb != -1) {
        char *backColorSpec = (char *) malloc(sizeof(char) * 19);
        if (!backColorSpec) {
            perror("Error while allocating colorSpec");
            exit(-1);
        }
        sprintf(backColorSpec, BG_RGB, br, bg, bb);
        printf("%s", backColorSpec);
        free(backColorSpec);
    }
    
    printf("%s", str);
    printf("%s", RESET);
}

void println_rgb(const char *str, unsigned char fr, unsigned char fg, unsigned char fb, unsigned char br, unsigned char bg, unsigned char bb) {
    print_rgb(str, fr, fg, fb, br, bg, bb);
    printf("\n");
}

#endif