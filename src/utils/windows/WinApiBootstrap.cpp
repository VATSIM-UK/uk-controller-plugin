#include "pch/pch.h"
#include "windows/WinApiBootstrap.h"
#include "WinApi.h"
#include "data/PluginDataLocations.h"

namespace UKControllerPlugin {
    namespace Windows {

        /*
         * Get a WinApi instance
         */
        std::unique_ptr<WinApiInterface> Bootstrap(HINSTANCE instance)
        {
            return std::make_unique<WinApi>(
                instance,
                GetFullPluginDataRoot()
            );
        }
    } // namespace Windows
} // namespace UKControllerPlugin
