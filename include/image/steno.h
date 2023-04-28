#ifndef IMAGE_STENO_H
#define IMAGE_STENO_H

#include "utils.h"

typedef struct steno_metadata_t steno_metadata_t;
struct steno_metadata_t {
    size_t bits_per_byte;
    size_t data_length;
    size_t offset;
    size_t step;
};

steno_metadata_t *steno_metadata_t_alloc(size_t bits_per_byte, size_t data_length, size_t offset, size_t step);
void steno_metadata_t_free(steno_metadata_t *metadata);
void steganography_encode(image_t *img, unsigned char *data, steno_metadata_t *metadata);
void steganography_decode(image_t *img, unsigned char *data, steno_metadata_t *metadata);

steno_metadata_t *steno_metadata_t_alloc(size_t bits_per_byte, size_t data_length, size_t offset, size_t step) {
    steno_metadata_t *metadata = (steno_metadata_t *) malloc(sizeof(steno_metadata_t));
    if (!metadata) {
        perror("Error while allocating steno metadata");
        exit(-1);
    }

    metadata->bits_per_byte = bits_per_byte;
    metadata->data_length = data_length;
    metadata->offset = offset;
    metadata->step = step;

    return metadata;
}

void steno_metadata_t_free(steno_metadata_t *metadata) {
    free(metadata);
}

void steganography_encode(image_t *img, unsigned char *data, steno_metadata_t *metadata) {
    size_t bits_per_byte = metadata->bits_per_byte, data_length = metadata->data_length, offset = metadata->offset, step = metadata->step;

    if (bits_per_byte > 4 && bits_per_byte <= 8)
        return;

    unsigned char *pPixels, *pData = data, buffer = *data, temp;
    int bitsLeftBuffer = 8, bitsLeftNextDataPos = 8;

    pPixels = img->pixels + (offset * sizeof(unsigned char));

    while (pData < (data + data_length)) {
        if (bitsLeftBuffer == 0) {
            pData++;

            bitsLeftBuffer = bitsLeftNextDataPos;
            buffer = *pData;

            buffer = buffer << (8 - bitsLeftBuffer);
            buffer = buffer >> (8 - bitsLeftBuffer);
        }
        if (bitsLeftBuffer < bits_per_byte) {
            temp = *(pData + 1);

            temp = temp >> (8 - (bits_per_byte - bitsLeftBuffer));

            buffer = buffer << (bits_per_byte - bitsLeftBuffer);
            buffer = buffer | temp;

            bitsLeftNextDataPos = (8 - (bits_per_byte - bitsLeftBuffer));
            bitsLeftBuffer = bits_per_byte;
        }

        *pPixels = *pPixels >> bits_per_byte;
        *pPixels = *pPixels << bits_per_byte;

        temp = buffer;
        temp = temp << (8 - (bitsLeftBuffer - bits_per_byte));
        temp = temp >> (8 - (bitsLeftBuffer - bits_per_byte));

        buffer = buffer >> (bitsLeftBuffer - bits_per_byte);

        *pPixels = *pPixels | buffer;

        buffer = temp;

        pPixels += step;
        bitsLeftBuffer -= bits_per_byte;
    }
}

void steganography_decode(image_t *img, unsigned char *data, steno_metadata_t *metadata) {
    size_t bits_per_byte = metadata->bits_per_byte, data_length = metadata->data_length, offset = metadata->offset, step = metadata->step;

    unsigned char *pPixels, *pData = data, buffer;
    size_t bitsToCompleteByte = 8, bitsLeftToRead = bits_per_byte;

    pPixels = img->pixels + (offset * sizeof(unsigned char));

    while (pData < (data + data_length)) {
        if (bitsLeftToRead == 0) {
            pPixels += step;
            bitsLeftToRead = bits_per_byte;
        }

        buffer = *pPixels;

        if (bitsToCompleteByte < bitsLeftToRead) {
            buffer = buffer << (8 - bitsToCompleteByte);
            buffer = buffer >> (8 - bitsToCompleteByte);

            *pData = *pData << bitsToCompleteByte;

            *pData = *pData | buffer;

            bitsLeftToRead -= bitsToCompleteByte;

            pData++;
            bitsToCompleteByte = 8;
            continue;
        }

        buffer = buffer << (8 - bitsLeftToRead);
        buffer = buffer >> (8 - bitsLeftToRead);

        *pData = *pData << bitsLeftToRead;
        *pData = *pData | buffer;

        bitsToCompleteByte -= bitsLeftToRead;
        bitsLeftToRead -= bitsLeftToRead;
    }
}

#endif