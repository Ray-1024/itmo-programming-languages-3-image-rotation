#include "image/transformations/rotate/rotate.h"
#include "stddef.h"

enum transform_status rotate(struct image from, struct image *to) {
    if (to == NULL)return TRANSFORM_ERROR;
    *to = image_create(from.h, from.w);
    if(image_is_invalid(*to))return TRANSFORM_ERROR;
    struct pixel px;
    for (uint64_t row = 0; row < from.h; ++row) {
        for (uint64_t column = 0; column < from.w; ++column) {
            px = image_get_pixel(from, row, column);
            image_set_pixel(*to, column, from.h - 1 - row, px);
        }
    }
    return TRANSFORM_OK;
}
