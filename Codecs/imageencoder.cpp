#include "imageencoder.h"
#include <fstream>
#include <filesystem>
#include "PPM/ppmencoder.h"

void ImageEncoder::Attach(std::shared_ptr<std::ostream> pStream)
{
    if (!pStream)
        throw std::invalid_argument("pStream");

    _pStream = pStream;
}

void ImageEncoder::Attach(const std::string &fileName)
{
    std::shared_ptr<std::ofstream> pStream(new std::ofstream(fileName, std::ios_base::binary | std::ios_base::out));
    if (!pStream->is_open())
        throw std::invalid_argument("pStream");

    Attach(pStream);
}

void ImageEncoder::Detach()
{
    _pStream.reset();
}

std::shared_ptr<ImageEncoder> ImageEncoder::Create(const std::string &fileName)
{
    auto path = std::filesystem::path(fileName);
    auto extension = path.extension();
    std::shared_ptr<ImageEncoder> pEncoder;
    if (extension == ".pgm" || extension == ".ppm")
    {
        pEncoder.reset(new PpmEncoder(PpmMode::Binary));
    }

    if (!pEncoder)
        throw std::invalid_argument("fileName");

    return pEncoder;
}
