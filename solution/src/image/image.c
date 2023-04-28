#include "image/image.h"
#include <malloc.h>

struct pixel image_get_pixel(const struct image image, uint64_t row, uint64_t column) {
    if(image_is_invalid(image))return (struct pixel){0};
    return image.data[row * image.w + column];
}

void image_set_pixel(struct image image, uint64_t row, uint64_t column, struct pixel newPixel) {
    if(image_is_invalid(image))return;
    image.data[row * image.w + column] = newPixel;
}

struct image image_create(uint64_t width, uint64_t height) {
    return (struct image) {.w=width, .h=height, .data=malloc(width * height * sizeof(struct pixel))};
}

void image_free(struct image image) {
    free(image.data);
}

bool image_is_invalid(struct image image){
    return image.data == NULL;
}
