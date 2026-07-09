#include "FrikSmoothMovement.h"

#include "GameHooks.h"

// This is the entry point to the mod.
extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_skse, F4SE::PluginInfo* a_info)
{
    return g_mod->onF4SEPluginQuery(a_skse, a_info);
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
