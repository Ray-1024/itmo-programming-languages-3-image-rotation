#ifndef IMAGE_TRANSFORMER_BMP24_H
#define IMAGE_TRANSFORMER_BMP24_H

#include "image/formats/bmp/bmp.h"
#include "image/image.h"

enum bmp_read_status bmp24_read_from_file(const char *filename, struct image *img);


enum bmp_write_status bmp24_write_to_file(const char *filename, struct image img);

#endif
