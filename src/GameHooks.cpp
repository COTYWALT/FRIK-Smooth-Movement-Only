#include "GameHooks.h"

#include "FrikSmoothMovement.h"
#include "f4vr/F4VROffsets.h"

namespace
{
    /**
     * Runs in place of the game's smooth-movement call: apply our smoothing, then chain to
     * the original function so the game keeps behaving normally.
     */
    void hookSmoothMovement(const std::uint64_t rcx)
    {
        frik_smooth_movement::g_frikSmoothMovement.updateSmoothMovement();
        f4vr::smoothMovementHook(rcx);
    }
}

namespace frik_smooth_movement::hook
{
    void installSmoothMovementHook()
    {
        auto& trampoline = F4SE::GetTrampoline();
        trampoline.write_call<5>(f4vr::hook_smoothMovementHook.address(), &hookSmoothMovement);
    }
}
