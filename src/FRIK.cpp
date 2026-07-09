#include "FRIK.h"

#include <Version.h>
#include <filesystem>
#include <spdlog/sinks/basic_file_sink.h>

#include "Config.h"
#include "GameHooks.h"

namespace
{
    void removeOldRotatedFrikLogs(const std::filesystem::path& logPath)
    {
        const auto dir = logPath.parent_path();
        const auto stem = logPath.stem().string();
        const auto ext = logPath.extension().string();

        for (int i = 1; i <= 20; ++i) {
            std::error_code ec;
            std::filesystem::remove(dir / fmt::format("{}.{}{}", stem, i, ext), ec);
        }
    }

    void initSingleFileLogger()
    {
        auto path = F4SE::log::log_directory();
        const auto gamePath = REL::Module::IsVR() ? "Fallout4VR/F4SE" : "Fallout4/F4SE";
        if (!path.value().generic_string().ends_with(gamePath)) {
            path = path.value().parent_path().append(gamePath);
        }

        *path /= "FRIK.log";
        std::filesystem::create_directories(path->parent_path());
        removeOldRotatedFrikLogs(*path);

        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

        f4cf::logger::internal::_logger = std::make_shared<spdlog::logger>("GLOBAL", sink);
        f4cf::logger::internal::_logger->set_level(spdlog::level::info);
        f4cf::logger::internal::_logger->flush_on(spdlog::level::info);
        f4cf::logger::internal::_logger->set_formatter(std::make_unique<f4cf::logger::internal::HybridFormatter>());
        spdlog::set_default_logger(f4cf::logger::internal::_logger);

        f4cf::logger::internal::_rawLogger = std::make_shared<spdlog::logger>(std::string(f4cf::logger::internal::RAW_LOGGER_NAME), sink);
        f4cf::logger::internal::_rawLogger->set_level(spdlog::level::info);
        f4cf::logger::internal::_rawLogger->flush_on(spdlog::level::info);
    }
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
    try {
        initSingleFileLogger();

        a_info->infoVersion = F4SE::PluginInfo::kVersion;
        a_info->name = Version::PROJECT.data();
        std::string version{ Version::NAME };
        std::erase(version, '.');
        a_info->version = std::stoi(version);

        if (a_f4se->IsEditor()) {
            logger::critical("Loaded in editor, marking as incompatible");
            return false;
        }

        if (a_f4se->RuntimeVersion() < (REL::Module::IsF4() ? F4SE::RUNTIME_LATEST : F4SE::RUNTIME_LATEST_VR)) {
            logger::critical("Unsupported runtime version {}", a_f4se->RuntimeVersion().string());
            return false;
        }

        logger::info("FRIK smooth movement only query succeeded.");
        return true;
    } catch (const std::exception& e) {
        logger::error("Unhandled exception in F4SEPlugin_Query: {}", e.what());
        return false;
    }
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
    try {
        logger::info("Init CommonLibF4 F4SE...");
        F4SE::Init(a_f4se, false);

        logger::info("Load smooth movement config...");
        frik::g_config.load();

        F4SE::AllocTrampoline(64);

        logger::info("Hook smooth movement only...");
        frik::hook::hookMain();

        logger::info("Smooth movement only build loaded.");
        return true;
    } catch (const std::exception& e) {
        logger::error("Unhandled exception in F4SEPlugin_Load: {}", e.what());
        return false;
    }
}

namespace frik
{
    void FRIK::smoothMovement()
    {
        try {
            _smoothMovement.onFrameUpdate();
        } catch (const std::exception& e) {
            logger::error("Error in FRIK::smoothMovement: {}", e.what());
        }
    }
}
