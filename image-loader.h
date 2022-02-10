#pragma once

#ifndef __IMAGE_LAODER_H__
#define __IMAGE_LAODER_H__

#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    NOT_SUCH_FILE      = -1115,
    CORRUPT_JPEG       = -1114,
    DECOMPRESS_SUCCESS = 0,
    COMPRESS_SUCCESS   = 0
};

struct CropRect
{
    int w;
    int h;
    int x;
    int y;
};

enum PixelFormat
{
    RGBA = 7
};

struct Image
{
public:
    PixelFormat Format;
    int Width;
    int Height;

public:
    void request_memory(int w, int h, int channel)
    {
        Width = w;
        Height = h;

        size = (w + (w & 0xf) * 2 /* for padding */) * h * channel;
        data.reset(new uint8_t[size]);
    }

    uint8_t *buffer()
    {
        return data.get();
    }

private:
    size_t size;
    std::unique_ptr<uint8_t> data;
};

int decompress_image(Image *image, const uint8_t *data, size_t size, CropRect *rect);

int compress_image(uint8_t **pData, size_t *size, Image *image);

#ifdef __cplusplus
}
#endif

#endif
