#pragma once

#include "smooth-movement/SmoothMovementVR.h"

namespace frik
{
    class FRIK
    {
    public:
        void smoothMovement();

    private:
        SmoothMovementVR _smoothMovement;
    };

    inline FRIK g_frik;
}
