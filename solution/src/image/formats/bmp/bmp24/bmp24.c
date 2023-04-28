#include "image/formats/bmp/bmp24/bmp24.h"
#include <stdio.h>

#define BMP24_PADDING 4
#define BMP24_DEFAULT_BFTYPE 19778
#define BMP24_DEFAULT_BISIZE 40
#define BMP24_DEFAULT_BIPLANES 1
#define BMP24_DEFAULT_BITCOUNT 24
#define BMP24_DEFAULT_BIXPELSPERMETER 3780
#define BMP24_DEFAULT_BIYPELSPERMETER 3780


inline static enum bmp_read_status
bmp_read_header_from_file(FILE *file, struct bmp_header *header) {
    if (header == NULL)return BMP_READ_INVALID_HEADER;
    if (fread(header, sizeof(struct bmp_header), 1, file) != 1)
        return BMP_READ_INVALID_SIGNATURE;
    return BMP_READ_OK;
}

inline static size_t bmp_get_buffersize_for_padding(const struct image image){
    return (BMP24_PADDING - (image.w * sizeof(struct pixel)) % BMP24_PADDING)%BMP24_PADDING;
}

inline static enum bmp_read_status
bmp_read_pixels_from_file(FILE *file, struct image *image) {
    const size_t buff_size = bmp_get_buffersize_for_padding(*image);
    int32_t tmpBuffer;
    for (size_t i = 0; i < image->h; ++i) {
        if (fread(image->data + image->w * i, sizeof(struct pixel), image->w, file) != image->w)
            return BMP_READ_INVALID_BITS;
        if (buff_size != 0 && fread(&tmpBuffer, buff_size, 1, file) != 1)
            return BMP_READ_INVALID_BITS;
    }
    return BMP_READ_OK;
}

static enum bmp_read_status bmp24_read_from_opened_file(FILE *file, struct image *image) {
    struct bmp_header header;
    enum bmp_read_status status = bmp_read_header_from_file(file, &header);
    if (status != BMP_READ_OK)return status;
    *image = image_create(header.biWidth, header.biHeight);
    return bmp_read_pixels_from_file(file, image);
}

enum bmp_read_status bmp24_read_from_file(const char *filename, struct image *img) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL)return BMP_READ_INVALID_SIGNATURE;
    enum bmp_read_status status = bmp24_read_from_opened_file(file, img);
    fclose(file);
    return status;
}

static struct bmp_header create_default_header(const struct image image) {
    struct bmp_header header = {0};
    header.bfType = BMP24_DEFAULT_BFTYPE;
    header.bfileSize =
            sizeof(struct bmp_header) +
            (((image.w * sizeof(struct pixel) + BMP24_PADDING - 1) / BMP24_PADDING) * BMP24_PADDING) * image.h;
    header.bOffBits = sizeof(struct bmp_header);
    header.biSize = BMP24_DEFAULT_BISIZE;
    header.biWidth = image.w;
    header.biHeight = image.h;
    header.biPlanes = BMP24_DEFAULT_BIPLANES;
    header.biBitCount = BMP24_DEFAULT_BITCOUNT;
    header.biSizeImage = header.bfileSize - header.bOffBits;
    header.biXPelsPerMeter = BMP24_DEFAULT_BIXPELSPERMETER;
    header.biYPelsPerMeter = BMP24_DEFAULT_BIYPELSPERMETER;
    return header;
}


inline static enum bmp_write_status bmp_write_header_to_file(FILE *file, const struct image image) {
    struct bmp_header header = create_default_header(image);
    if (fwrite(&header, sizeof(struct bmp_header), 1, file) != 1)
        return BMP_WRITE_ERROR;
    return BMP_WRITE_OK;
}

inline static enum bmp_write_status bmp_write_pixels_to_file(FILE *file, const struct image image) {
    const size_t buff_size = bmp_get_buffersize_for_padding(image);
    int32_t tmpBuffer = 0;
    for (uint64_t row = 0; row < image.h; ++row) {
        if (fwrite(image.data + image.w * row, sizeof(struct pixel), image.w, file) != image.w)
            return BMP_WRITE_ERROR;
        if (buff_size != 0 && fwrite(&tmpBuffer, buff_size, 1, file) != 1)
            return BMP_WRITE_ERROR;
    }
    return BMP_WRITE_OK;
}


static enum bmp_write_status bmp24_write_to_opened_file(FILE *out, const struct image img) {
    if (out == NULL)return BMP_WRITE_ERROR;
    enum bmp_write_status status = bmp_write_header_to_file(out, img);
    if (status != BMP_WRITE_OK)return status;
    return bmp_write_pixels_to_file(out, img);
}


enum bmp_write_status bmp24_write_to_file(const char *filename, struct image img) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL)return BMP_WRITE_ERROR;
    enum bmp_write_status status = bmp24_write_to_opened_file(file, img);
    fclose(file);
    return status;
}
