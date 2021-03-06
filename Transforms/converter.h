#ifndef CONVERTER_H
#define CONVERTER_H
#include "../Core/bitmap.h"
#include <stdexcept>
#include "basetransform.h"
#include "../Core/IParallel.h"

template<PixelFormat srcPixelFormat, PixelFormat dstPixelFormat>
void ConvertPixel(std::conditional_t<BytesPerChannel(srcPixelFormat) == 1, uint8_t*, uint16_t*>, std::conditional_t<BytesPerChannel(srcPixelFormat) == 1, uint8_t*, uint16_t*>)
{
    throw std::runtime_error("not implemented");
}

template<>
void ConvertPixel<PixelFormat::RGB24, PixelFormat::Gray8>(uint8_t* pSrcPixel, uint8_t* pDstPixel);

template<>
void ConvertPixel<PixelFormat::RGB48, PixelFormat::Gray8>(uint16_t* pSrcPixel, uint16_t* pDstPixel);

class BaseConverter : public BaseTransform, public IParallel
{
protected:
    BaseConverter(IBitmapPtr pSrcBitmap);

public:
    static std::shared_ptr<BaseConverter> Create(IBitmapPtr pSrcBitmap, PixelFormat dstPixelFormat);
    static IBitmapPtr Convert(IBitmapPtr pSrcBitmap, PixelFormat dstPixelFormat);
};

template <PixelFormat srcPixelFormat, PixelFormat dstPixelFormat>
class Converter : public BaseConverter
{
public:
    Converter(IBitmapPtr pSrcBitmap)
    : BaseConverter(pSrcBitmap)
    {

    }

    void Run() override
    {
        _pDstBitmap = std::make_shared<Bitmap<dstPixelFormat>>(_pSrcBitmap->GetWidth(), _pSrcBitmap->GetHeight());
        DoParallelJobs();
        /*for (uint32_t i = 0; i < _threadCount; ++i)
        {
            Job(i);
        }*/
    }

    void Job(uint32_t i) override
    {
        auto pSrcBitmap = std::static_pointer_cast<Bitmap<srcPixelFormat>>(_pSrcBitmap);
        auto pDstBitmap = std::static_pointer_cast<Bitmap<dstPixelFormat>>(_pDstBitmap);

        auto pSrcScanline = pSrcBitmap->GetScanline(i);
        auto pDstScanline = pDstBitmap->GetScanline(i);

        for (uint32_t j = 0; j < pDstBitmap->GetWidth(); ++j)
        {
            ConvertPixel<srcPixelFormat, dstPixelFormat>(pSrcScanline, pDstScanline);
            pSrcScanline += ChannelCount(srcPixelFormat);
            pDstScanline += ChannelCount(dstPixelFormat);
        }        
    }
};

#endif // CONVERTER_H
