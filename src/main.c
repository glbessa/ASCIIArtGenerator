#include "image/ascii.h"
#include "image/steno.h"
#include "image/type_conv.h"
#include "image/utils.h"

#include "string_extra.h"
#include "color_term.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void print_help();

int main(int argc, char **argv)
{
    convert_directory("./never_gonna", "./never_gonna_ascii", 320, 180, ASCII);

    return 0;
}

void print_help() {
    printf("Help message\n");
}