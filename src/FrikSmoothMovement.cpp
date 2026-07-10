#include "FrikSmoothMovement.h"

#include <chrono>
#include <filesystem>
#include <fmt/chrono.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "GameHooks.h"

namespace
{
    std::filesystem::path getLogPath(const std::string_view& logFileName)
    {
        auto path = F4SE::log::log_directory();
        const auto gamePath = REL::Module::IsVR() ? "Fallout4VR/F4SE" : "Fallout4/F4SE";
        if (!path.value().generic_string().ends_with(gamePath)) {
            // handle bug where game directory is missing
            path = path.value().parent_path().append(gamePath);
        }

        std::filesystem::create_directories(path.value());
        return path.value() / fmt::format("{}.log"sv, logFileName);
    }

    void deleteRotatedLogs(const std::filesystem::path& logPath)
    {
        const auto directory = logPath.parent_path();
        const auto stem = logPath.stem().string();
        const auto extension = logPath.extension().string();

        for (int i = 1; i <= 20; ++i) {
            std::error_code error;
            std::filesystem::remove(directory / fmt::format("{}.{}{}", stem, i, extension), error);
        }
    }

    void initSingleLog(const std::string_view& logFileName)
    {
        const auto logPath = getLogPath(logFileName);
        deleteRotatedLogs(logPath);

        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath.string(), true);

        f4cf::logger::internal::_logger = std::make_shared<spdlog::logger>("GLOBAL"s, sink);
        f4cf::logger::internal::_logger->set_level(spdlog::level::info);
        f4cf::logger::internal::_logger->flush_on(spdlog::level::info);
        f4cf::logger::internal::_logger->set_formatter(std::make_unique<f4cf::logger::internal::HybridFormatter>());
        spdlog::set_default_logger(f4cf::logger::internal::_logger);

        f4cf::logger::internal::_rawLogger = std::make_shared<spdlog::logger>(std::string(f4cf::logger::internal::RAW_LOGGER_NAME), sink);
        f4cf::logger::internal::_rawLogger->set_level(spdlog::level::info);
        f4cf::logger::internal::_rawLogger->flush_on(spdlog::level::info);
    }

    void logPluginGameStart()
    {
        const auto game = REL::Module::IsVR() ? "Fallout4VR" : "Fallout4";
        const auto runtimeVer = REL::Module::get().version();
        const auto dateTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logger::info("Starting '{}' v{} ; {} v{} ; {:%Y-%m-%d %H:%M:%S%Ez} ; BaseAddress: 0x{:X}",
            Version::PROJECT,
            Version::NAME,
            game,
            runtimeVer.string(),
            fmt::localtime(dateTime),
            REL::Module::get().base());
    }

    bool queryPlugin(const F4SE::QueryInterface* f4se, F4SE::PluginInfo* info)
    {
        bool success = false;
        try {
            initSingleLog(Version::PROJECT);
            logPluginGameStart();

            info->infoVersion = F4SE::PluginInfo::kVersion;
            info->name = Version::PROJECT.data();
            std::string version = std::string(Version::NAME);
            std::erase(version, '.');
            info->version = std::stoi(version);

            if (f4se->IsEditor()) {
                logger::critical("Loaded in editor, marking as incompatible");
            } else if (f4se->RuntimeVersion() < (REL::Module::IsF4() ? F4SE::RUNTIME_LATEST : F4SE::RUNTIME_LATEST_VR)) {
                logger::critical("Unsupported runtime version {}", f4se->RuntimeVersion().string());
            } else {
                logger::info("F4SE Plugin Query v{} compatible: {} v{}", f4se->F4SEVersion().string(), info->name, info->version);
                success = true;
            }
        } catch (const std::exception& ex) {
            if (f4cf::logger::internal::_logger) {
                logger::error("Unhandled exception: {}", ex.what());
            }
        }

        return success;
    }
}

// This is the entry point to the mod.
extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_skse, F4SE::PluginInfo* a_info)
{
    return queryPlugin(a_skse, a_info);
}

// This is the entry point to the mod.
extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
    return g_mod->onF4SEPluginLoad(a_f4se);
}

namespace frik_smooth_movement
{
    /**
     * Run F4SE plugin load and initialize the plugin given the init handle.
     * Install the dedicated smooth-movement hook once the trampoline is ready.
     */
    void FrikSmoothMovement::onModLoaded(const F4SE::LoadInterface*)
    {
        logger::info("Hook smooth movement...");
        hook::installSmoothMovementHook();
    }

    /**
     * On game fully loaded initialize things that should be initialized only once.
     */
    void FrikSmoothMovement::onGameLoaded()
    {
        //noop
    }

    /**
     * Game session can be initialized multiple times as it is fired on new game and save loaded events.
     */
    void FrikSmoothMovement::onGameSessionLoaded()
    {
        //noop
    }

    /**
     * Unused: smoothing runs from the dedicated movement hook, not the generic frame loop
     * (setupMainGameLoop is off), so this never fires.
     */
    void FrikSmoothMovement::onFrameUpdate()
    {
        //noop
    }

    /**
     * Drive the smoothing on the game's movement tick. Runs on the game thread outside of
     * ModBase's exception wrapper, so guard it here.
     */
    void FrikSmoothMovement::updateSmoothMovement()
    {
        try {
            _smoothMovement.onFrameUpdate();
        } catch (const std::exception& e) {
            logger::error("Error in updateSmoothMovement: {}", e.what());
        }
    }
}
