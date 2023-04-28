#ifndef IMAGE_TRANSFORMER_UTILS_H
#define IMAGE_TRANSFORMER_UTILS_H

#include "image/image.h"
#include "image/transformations/transform.h"


void
read_transform_write(image_read_from_file read, image_transform transform, image_write_to_file write, const char *from,
                     const char *to);


#endif
