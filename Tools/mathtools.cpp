 #include "mathtools.h"

float QuadraticInterpolation(float t, float t0, float t1, float t2)
{
    auto a = (t0 + t2) / 2 - t1;
    auto b = -(3 * t0 + t2) / 2 + 2 * t1;
    //auto c = t0;
    auto res = a * t * t + b * t + t0;
    return res;
}
