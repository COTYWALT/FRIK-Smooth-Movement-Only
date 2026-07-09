#pragma once

#include "ConfigBase.h"
#include "Resources.h"
#include "common/CommonUtils.h"

namespace frik_smooth_movement
{
    static const auto BASE_MOD_PATH = BASE_PATH + "\\" + std::string(Version::PROJECT);
    static const auto INI_PATH = BASE_MOD_PATH + "\\" + std::string(Version::PROJECT) + ".ini";

    class Config : public ConfigBase
    {
    public:
        explicit Config()
            : ConfigBase(Version::PROJECT, INI_PATH, IDR_CONFIG_INI)
        {}

        // Smooth movement settings (vertical / up-down)
        float smoothingAmount = 5.0f;
        float dampingMultiplier = 1.0f;
        float stoppingMultiplier = 0.5f;

        // Smooth movement settings (horizontal)
        float smoothingAmountHorizontal = 5.0f;
        float dampingMultiplierHorizontal = 1.0f;
        float stoppingMultiplierHorizontal = 0.5f;

    protected:
        virtual void loadIniConfigInternal(const CSimpleIniA& ini) override;
    };

    // Global singleton for easy access
    inline Config g_config;
}
