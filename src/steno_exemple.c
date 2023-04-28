/*
#include "image/steno.h"
#include "image/utils.h"

#include "color_term.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    steno_metadata_t *metadata = steno_metadata_t_alloc(4, 17, 200, 3);

    image_t *img = image_load("test.png", 0);
    unsigned char *text = malloc(sizeof(unsigned char) * (metadata->data_length + 1));

    steganography_encode(img, (unsigned char *) "Ola mundo cruel\n", metadata);
    steganography_decode(img, text, metadata);

    printf("%s%s%s", get_ansi_rgb_foreground(34, 212, 54), text, RESET);

    image_save("test2.png", img, PNG);

    image_t_free(img);

    return 0;
}
*/