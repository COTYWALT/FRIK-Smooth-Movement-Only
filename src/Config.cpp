#include "Config.h"

#include <SimpleIni.h>
#include <filesystem>
#include <fstream>

namespace
{
    constexpr std::string_view DEFAULT_FRIK_INI = R"ini([SmoothMovementVR]

# Vertical smoothing. Higher values mask more up/down stutter.
SmoothAmount = 5.0
Damping = 1.0
StoppingMultiplier = 0.5

# Horizontal smoothing. Higher values mask more horizontal stutter.
SmoothAmountHorizontal = 5.0
DampingHorizontal = 1.0
StoppingMultiplierHorizontal = 0.5
)ini";
}

namespace frik
{
    void Config::load()
    {
        setupFolders();

        CSimpleIniA ini;
        SI_Error rc = ini.LoadFile(FRIK_INI_PATH.c_str());
        if (rc < 0) {
            logger::warn("Smooth movement INI not found at '{}'; creating default config.", FRIK_INI_PATH);
            writeDefaultIni();
            rc = ini.LoadFile(FRIK_INI_PATH.c_str());
            if (rc < 0) {
                logger::warn("Failed to load created smooth movement INI at '{}'; using built-in defaults.", FRIK_INI_PATH);
                return;
            }
        }

        smoothingAmount = static_cast<float>(ini.GetDoubleValue(INI_SECTION_SMOOTH_MOVEMENT, "SmoothAmount", 5.0));
        dampingMultiplier = static_cast<float>(ini.GetDoubleValue(INI_SECTION_SMOOTH_MOVEMENT, "Damping", 1.0));
        stoppingMultiplier = static_cast<float>(ini.GetDoubleValue(INI_SECTION_SMOOTH_MOVEMENT, "StoppingMultiplier", 0.5));
        smoothingAmountHorizontal = static_cast<float>(ini.GetDoubleValue(INI_SECTION_SMOOTH_MOVEMENT, "SmoothAmountHorizontal", 5.0));
        dampingMultiplierHorizontal = static_cast<float>(ini.GetDoubleValue(INI_SECTION_SMOOTH_MOVEMENT, "DampingHorizontal", 1.0));
        stoppingMultiplierHorizontal = static_cast<float>(ini.GetDoubleValue(INI_SECTION_SMOOTH_MOVEMENT, "StoppingMultiplierHorizontal", 0.5));
    }

    void Config::setupFolders()
    {
        std::filesystem::create_directories(std::filesystem::path(FRIK_INI_PATH).parent_path());
    }

    void Config::writeDefaultIni()
    {
        std::ofstream out(FRIK_INI_PATH, std::ios::out | std::ios::trunc);
        if (!out) {
            logger::warn("Failed to create default smooth movement INI at '{}'.", FRIK_INI_PATH);
            return;
        }

        out << DEFAULT_FRIK_INI;
    }
}
