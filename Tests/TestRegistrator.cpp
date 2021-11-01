#include "test.h"
#include "testtools.h"
#include "../Registrator/registrator.h"

BEGIN_SUITE(Registrator)

BEGIN_TEST(Registrator, BasicTest)

auto pRegistrator = std::make_unique<Registrator>();
auto dataset = pRegistrator->Registrate(IBitmap::Create(GetPathToTestFile("PPM/IMG_4030.ppm")));
auto& stars = dataset->stars;
EXPECT_EQ(675, dataset->starCount);
EXPECT_EQ(1.0, stars[0].luminance);
EXPECT_EQ(151, stars[0].pixelCount);
EXPECT_EQ((Rect{327, 229, 12, 20}), stars[0].rect);

END_TEST

BEGIN_TEST(Registrator, RegistrateHugePhoto)
auto pRegistrator = std::make_unique<Registrator>(50);
auto dataset = pRegistrator->Registrate(IBitmap::Create(GetPathToTestFile("PPM/IMG_4314.ppm")));
auto& stars = dataset->stars;
EXPECT_EQ(8971, dataset->starCount);
EXPECT_EQ(196, stars[2].pixelCount);
EXPECT_NEAR(0.89, stars[2].luminance, 0.01);
EXPECT_EQ((Rect{1336, 1782, 11, 20}), stars[2].rect);
EXPECT_EQ(21, stars.size());
END_TEST

BEGIN_TEST(Registrator, TestVertical)
auto pRegistrator = std::make_unique<Registrator>(40);
auto dataset = pRegistrator->Registrate(IBitmap::Create(GetPathToTestFile("PPM/vertical.ppm")));
auto& stars = dataset->stars;
EXPECT_EQ(1654, dataset->starCount);
EXPECT_EQ(115, stars[1].pixelCount);
EXPECT_NEAR(0.46, stars[1].luminance, 0.01);
EXPECT_EQ((Rect{648, 129, 11, 11}), stars[1].rect);
END_TEST

END_SUITE
