#include "stdafx.h"
#include "Math.h"

static std::random_device rd;
static std::mt19937_64 mt(rd());

auto Math::Random(const float & min, const float & max) -> const float
{
    return std::uniform_real_distribution<float>{min, max}(mt);
}
