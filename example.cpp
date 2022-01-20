#include "image-loader.h"

#include <iostream>
#include <string>
#include <cstdlib>

#define _UNIX03_SOURCE
#include <dlfcn.h>

using PFN_decompress_image = int (*)(Image *image, const uint8_t *data, size_t size, CropRect *rect);
using PFN_compress_image   = int (*)(uint8_t **pData, size_t *size, Image *image);

class DLLController
{
public:
    DLLController(const std::string &path)
    {
        handle = dlopen(path.c_str(), RTLD_LAZY);
    }

    template <class T>
    T get_func(const std::string &func_name)
    {
        return reinterpret_cast<T>(dlsym(handle, func_name.c_str()));
    }

    ~DLLController()
    {
        dlclose(handle);
    }

private:
    void *handle;
};

static std::vector<uint8_t> load_image_from_file(const std::string &path)
{
    FILE *fp = fopen(path.c_str(), "rb+");
    if (!fp)
    {
        throw("No such file: " + path);
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    std::vector<uint8_t> raw_jpeg_data;
    raw_jpeg_data.resize(size);
    fread(raw_jpeg_data.data(), 1, size, fp);

    fclose(fp);

    return raw_jpeg_data;
}

static void save_file(const std::string &path, uint8_t *data, size_t size)
{
    FILE *fp = fopen(path.c_str(), "wb+");
    if (!fp)
    {
        throw("Corrupted: " + path);
    }

    fwrite(data, size, 1, fp);

    fclose(fp);
}

int main()
{
    auto dllController = DLLController("./build/libimage-loader.so");

    auto decompress_image = dllController.get_func<PFN_decompress_image>("decompress_image");
    auto compress_image   = dllController.get_func<PFN_compress_image>("compress_image");

    Image image{};
    CropRect rect{ 3840, 2160, 0, 0 };

    auto jpeg = load_image_from_file("../Assets/4k_sam.jpg");

    decompress_image(&image, jpeg.data(), jpeg.size(), &rect);

    uint8_t *buf = nullptr;
    size_t size = 0;

    compress_image(&buf, &size, &image);

    save_file("test.jpg", buf, size);
    return 0;
}
