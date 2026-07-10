#pragma once

#include "Config.h"
#include "ModBase.h"
#include "smooth-movement/SmoothMovementVR.h"

namespace frik_smooth_movement
{
    /**
     * Standalone Fallout 4 VR mod that applies only FRIK's smooth-movement smoothing.
     *
     * The smoothing must run at the game's dedicated movement call site, so instead of the
     * generic per-frame loop this mod installs its own hook (see GameHooks) and drives
     * SmoothMovementVR from there. All other lifecycle plumbing (logging, config hot-reload,
     * F4SE registration) comes from ModBase.
     */
    class FrikSmoothMovement : public ModBase
    {
    public:
        FrikSmoothMovement()
            : ModBase(Settings(Version::PROJECT, Version::NAME, &g_config, 64, false))
        {}

        // Called from the dedicated smooth-movement hook on every movement tick.
        void updateSmoothMovement();

    protected:
        virtual void onModLoaded(const F4SE::LoadInterface* f4SE) override;
        virtual void onGameLoaded() override;
        virtual void onGameSessionLoaded() override;
        virtual void onFrameUpdate() override;

    private:
        SmoothMovementVR _smoothMovement;
    };

    // The ONE global to rule them ALL
    inline FrikSmoothMovement g_frikSmoothMovement;
}
