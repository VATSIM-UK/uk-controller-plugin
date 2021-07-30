#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An interface for classes that want to know when someone saves the EuroScope
            runway selection dialog.
        */
        class RunwayDialogAwareInterface
        {
            public:

                virtual void RunwayDialogSaved(void) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
