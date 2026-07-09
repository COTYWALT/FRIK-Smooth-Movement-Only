#include "GameHooks.h"

#include "FRIK.h"
#include "f4vr/F4VROffsets.h"

namespace
{
    void hookSmoothMovement(const std::uint64_t rcx)
    {
        frik::g_frik.smoothMovement();
        f4vr::smoothMovementHook(rcx);
    }
}

namespace frik::hook
{
    void hookMain()
    {
        auto& trampoline = F4SE::GetTrampoline();
        trampoline.write_call<5>(f4vr::hook_smoothMovementHook.address(), &hookSmoothMovement);
    }
}
