#ifndef REGISTRATOR_H
#define REGISTRATOR_H
#include "../Tests/testtools.h"
#include "../Transforms/converter.h"
#include "../Codecs/imagedecoder.h"
#include "../Core/IParallel.h"

#include <algorithm>

#include "star.h"

//class IBitmap;

class Registrator : public IParallel
{
public:
    static constexpr uint32_t tileSize = 600;

private:
    std::shared_ptr<IBitmap> _pBitmap;
    double _threshold = 40;
    uint32_t _minStarSize = 5;
    uint32_t _maxStarSize = 25;

    std::vector<std::vector<Star>> _stars;

public:
    Registrator() = default;
    Registrator( double threshold, uint32_t _minStarSize = 5, uint32_t _maxStarSize = 25);
    void Registrate(std::shared_ptr<IBitmap> pBitmap);
    const std::vector<std::vector<Star>>& GetStars() const;
    
    
private:

    virtual void Job(uint32_t i) override;

    template <PixelFormat pixelFormat>
    std::vector<Star> Registrate(Rect roi)
    {
        std::vector<Star> res;

        using ChannelType = typename PixelFormatTraits<pixelFormat>::ChannelType;
        auto pGrayBitmap = std::static_pointer_cast<Bitmap<pixelFormat>>(_pBitmap);
        //IBitmap::Save(pGrayBitmap, GetPathToTestFile("gray.pgm"));
        auto w = pGrayBitmap->GetWidth();
        auto h = pGrayBitmap->GetHeight();

        auto data = std::vector<ChannelType>(roi.width * roi.height * ChannelCount(pixelFormat));
        for (uint32_t i = 0; i < roi.height; ++i)
        {
            std::memcpy(&data[i * roi.width], pGrayBitmap->GetScanline(i + roi.y) + roi.x, roi.width * BytesPerPixel(pixelFormat));
        }

        auto median = data.begin() + data.size() / 2;
        std::nth_element(data.begin(), median, data.end());

        auto threshold = static_cast<ChannelType>(std::min(static_cast<uint32_t>(*median * (1 + _threshold / 100)), static_cast<uint32_t>(std::numeric_limits<ChannelType>::max())));

        auto pData = pGrayBitmap->GetScanline(0);
        

        for (uint32_t i = roi.y; i < roi.y + roi.height; ++i)
        {
            for (uint32_t j = roi.x; j < roi.x + roi.width; ++j)
            {
                if (pData[i * w + j] > threshold)
                {
                    Star star {Rect {static_cast<int32_t>(j), static_cast<int32_t>(i), 1, 1}, 0, 0, 0};
                    InspectStar(star, threshold, pData, j, i, w, h, roi);
                    if (star.rect.width >= _minStarSize && star.rect.width <= _maxStarSize && star.rect.height >= _minStarSize && star.rect.height <= _maxStarSize)
                    {
                        star.center.x /= star.luminance;
                        star.center.y /= star.luminance;
                        res.push_back(star);
                    }
                }
            }

        }

        //IBitmap::Save(pGrayBitmap, GetPathToTestFile("mask.pgm"));
        return res;
    }

    template <typename ChannelType>
    void InspectStar(Star& star, ChannelType threshold, ChannelType* pData, uint32_t x, uint32_t y, uint32_t w, uint32_t h, Rect roi)
    {
        ++star.pixelCount;        
        auto pixelLuminance = pData[y * w + x] - threshold;
        star.luminance += pixelLuminance;
        star.center.x += x * pixelLuminance;
        star.center.y += y * pixelLuminance;
        pData[y * w + x] = 0;

        if (star.rect.width > _maxStarSize + 1 || star.rect.height > _maxStarSize + 1)
            return;

        if (x + 1 < roi.x + roi.width && pData[y * w + x + 1] > threshold)
        {
            star.rect.ExpandRight(x + 1);
            InspectStar(star, threshold, pData, x + 1, y, w, h, roi);
        }

        if (x + 1 < roi.x + roi.width && y + 1 < roi.y + roi.height && pData[(y + 1) * w + x + 1] > threshold)
        {
            star.rect.ExpandRight(x + 1);
            star.rect.ExpandDown(y + 1);
            InspectStar(star, threshold, pData, x + 1, y + 1, w, h, roi);
        }

        if (y + 1 < roi.y + roi.height && pData[(y + 1) * w + x] > threshold)
        {
            star.rect.ExpandDown(y + 1);
            InspectStar(star, threshold, pData, x, y + 1, w, h, roi);
        }

        if (x > roi.x && y + 1 < roi.y + roi.height && pData[(y + 1) * w + x - 1] > threshold)
        {
            star.rect.ExpandDown(y + 1);
            star.rect.ExpandLeft(x - 1);
            InspectStar(star, threshold, pData, x - 1, y + 1, w, h, roi);
        }
    }
};

#endif
