#include "Config.h"

using namespace common;

namespace
{
    const char* DEFAULT_SECTION = Version::PROJECT.data();
}

namespace frik_smooth_movement
{
    void Config::loadIniConfigInternal(const CSimpleIniA& ini)
    {
        smoothingAmount = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fSmoothingAmount", 5.0));
        dampingMultiplier = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fDamping", 1.0));
        stoppingMultiplier = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fStoppingMultiplier", 0.5));

        smoothingAmountHorizontal = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fSmoothingAmountHorizontal", 5.0));
        dampingMultiplierHorizontal = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fDampingHorizontal", 1.0));
        stoppingMultiplierHorizontal = static_cast<float>(ini.GetDoubleValue(DEFAULT_SECTION, "fStoppingMultiplierHorizontal", 0.5));
    }
}
