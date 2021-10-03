#include "test.h"
#include "testtools.h"
#include "Codecs/PPM/ppmdecoder.h"
#include "Core/bitmap.h"
#include <cstring>

BEGIN_SUITE( PpmDecoder )

BEGIN_TEST(PpmDecoder, TestGray8)
    auto pDecoder = std::make_unique<PpmDecoder>();
    pDecoder->Attach(GetPathToTestFile("PPM/gray8.pgm"));
    EXPECT_EQ(PixelFormat::Gray8, pDecoder->GetPixelFormat());
    EXPECT_EQ(500, pDecoder->GetWidth());
    EXPECT_EQ(500, pDecoder->GetHeight());
END_TEST

BEGIN_TEST(PpmDecoder, TestGray16)
    auto pDecoder = std::make_unique<PpmDecoder>();
    pDecoder->Attach(GetPathToTestFile("PPM/gray16.pgm"));
    EXPECT_EQ(PixelFormat::Gray16, pDecoder->GetPixelFormat());
    EXPECT_EQ(500, pDecoder->GetWidth());
    EXPECT_EQ(500, pDecoder->GetHeight());
END_TEST

BEGIN_TEST(PpmDecoder, TestRgb24)
    auto pDecoder = std::make_unique<PpmDecoder>();
    pDecoder->Attach(GetPathToTestFile("PPM/rgb24.ppm"));
    EXPECT_EQ(PixelFormat::RGB24, pDecoder->GetPixelFormat());
    EXPECT_EQ(640, pDecoder->GetWidth());
    EXPECT_EQ(426, pDecoder->GetHeight());
END_TEST

BEGIN_TEST(PpmDecoder, TestRgb48)
    auto pDecoder = std::make_unique<PpmDecoder>();
    pDecoder->Attach(GetPathToTestFile("PPM/rgb48.ppm"));
    EXPECT_EQ(PixelFormat::RGB48, pDecoder->GetPixelFormat());
    EXPECT_EQ(500, pDecoder->GetWidth());
    EXPECT_EQ(500, pDecoder->GetHeight());
END_TEST

BEGIN_TEST(PpmDecoder, TestPlain)
    auto pDecoder = std::make_unique<PpmDecoder>();

    std::string fileNames[2] = { GetPathToTestFile("PPM/plain.ppm"), GetPathToTestFile("PPM/binary.ppm") };

    for (auto& fileName : fileNames)
    {
        pDecoder->Attach(fileName);
        EXPECT_EQ(PixelFormat::RGB24, pDecoder->GetPixelFormat());
        EXPECT_EQ(4, pDecoder->GetWidth());
        EXPECT_EQ(4, pDecoder->GetHeight());

        auto pBitmap = pDecoder->GetBitmap();
        EXPECT_EQ(PixelFormat::RGB24, pBitmap->GetPixelFormat());
        EXPECT_EQ(4, pBitmap->GetWidth());
        EXPECT_EQ(4, pBitmap->GetHeight());

        const std::vector<uint8_t> expectedData
        {
            0, 0, 0,      100, 0, 0,      0, 0, 0,        255, 0, 255,
            0, 0, 0,      0, 255, 175,    0, 0, 0,        0, 0, 0,
            0, 0, 0,      0, 0, 0,        0, 15, 175,     0, 0, 0,
            255, 0, 255,  0, 0, 0, 0,     0, 0, 255,      255, 255
        };

        EXPECT_EQ(0, memcmp(std::static_pointer_cast<Bitmap<PixelFormat::RGB24>>(pBitmap)->GetPlanarScanline(0), expectedData.data(), expectedData.size()));
    }
END_TEST

BEGIN_TEST(PpmDecoder, TestByteOrdering)
    auto pDecoder = std::make_unique<PpmDecoder>();
    pDecoder->Attach(GetPathToTestFile("PPM/IMG_4030.ppm"));
    auto pBitmap = pDecoder->GetBitmap();
    auto pScanLine = pBitmap->GetPlanarScanline(0);
    char bytes[2] = {pScanLine[0], pScanLine[1]};
    EXPECT_EQ(0x63, bytes[0]);
    EXPECT_EQ(0x7b, bytes[1]);

    auto pGray16Bitmap = std::static_pointer_cast<Bitmap<PixelFormat::Gray16>>(pBitmap);
    auto pix = pGray16Bitmap->GetChannel(0,0,0);
    EXPECT_EQ(0x637b, pix);

END_TEST

END_SUITE
