#pragma once

#include "ConfigBase.h"
#include "Resources.h"
#include "common/CommonUtils.h"

#include <filesystem>

namespace frik_smooth_movement
{
    inline std::string getPluginIniPath()
    {
        HMODULE module = nullptr;
        if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCSTR>(&getPluginIniPath),
                &module)) {
            throw std::runtime_error("Failed to locate FRIK-SmoothMovement.dll module");
        }

        char modulePath[MAX_PATH]{};
        if (GetModuleFileNameA(module, modulePath, MAX_PATH) == 0) {
            throw std::runtime_error("Failed to resolve FRIK-SmoothMovement.dll path");
        }

        return (std::filesystem::path(modulePath).parent_path() / (std::string(Version::PROJECT) + ".ini")).string();
    }

    class Config : public ConfigBase
    {
    public:
        explicit Config()
            : ConfigBase(Version::PROJECT, getPluginIniPath(), IDR_CONFIG_INI)
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
