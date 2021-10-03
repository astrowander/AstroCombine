#ifndef PPMDECODER_H
#define PPMDECODER_H

#include "Codecs/imagedecoder.h"
#include "Core/enums.h"
#include <iostream>
class PpmDecoder : public ImageDecoder
{
    PpmMode _ppmMode;
    uint32_t _maxval;
    std::shared_ptr<IBitmap> _pBitmap;

public:
    void Attach(const std::string& fileName) override;
    void Attach(std::shared_ptr<std::istream> pStream) override;
    std::shared_ptr<IBitmap> GetBitmap() override;    

private:

    template<uint32_t bytes>
    void ParseBinary();

    void ParseText();

    std::unique_ptr<std::istringstream> ReadLine() override
    {
        auto res = ImageDecoder::ReadLine();
        while (res->peek() == '#')
            res = ImageDecoder::ReadLine();

        return res;
    }
};

#endif // PPMDECODER_H
