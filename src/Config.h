#pragma once

#include <string>

namespace frik
{
    constexpr auto INI_SECTION_SMOOTH_MOVEMENT = "SmoothMovementVR";

    static const std::string FRIK_INI_PATH = R"(Data\F4SE\Plugins\FRIK.ini)";

    class Config
    {
    public:
        void load();

        float smoothingAmount = 5.0f;
        float smoothingAmountHorizontal = 5.0f;
        float dampingMultiplier = 1.0f;
        float dampingMultiplierHorizontal = 1.0f;
        float stoppingMultiplier = 0.5f;
        float stoppingMultiplierHorizontal = 0.5f;

    private:
        static void setupFolders();
        static void writeDefaultIni();
    };

    inline Config g_config;
}
