//#define GENERATE_PATTERNS
#include "testtools.h"
#include "../Core/bitmap.h"
#include <cstring>

bool BitmapsAreEqual(std::shared_ptr<IBitmap> lhs, std::shared_ptr<IBitmap> rhs)
{
    if
    (    lhs->GetPixelFormat() != rhs->GetPixelFormat() ||
            lhs->GetWidth() != rhs->GetWidth() ||
            lhs->GetHeight() != rhs->GetHeight() ||
            lhs->GetByteSize() != rhs->GetByteSize()
    )
        return false;

    return !static_cast<bool>(memcmp(lhs->GetPlanarScanline(0), rhs->GetPlanarScanline(0), lhs->GetByteSize()));
}

bool BitmapsAreEqual(const std::string& fileName, std::shared_ptr<IBitmap> rhs)
{
#ifdef GENERATE_PATTERNS
    IBitmap::Save(rhs, fileName);
    return true;
#endif
    return BitmapsAreEqual(IBitmap::Create(fileName), rhs);
}

const std::string testFilesPath {"./Tests/TestFiles/" };
const std::string patternsPath {"./Tests/Patterns/" };

std::string GetPathToPattern(const std::string &fileName)
{
    return patternsPath + fileName;
}

std::string GetPathToTestFile(const std::string &fileName)
{
    return testFilesPath + fileName;
}
