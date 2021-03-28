#pragma once

namespace UKControllerPlugin {
    namespace Windows {

        // Forward declare
        class WinApiInterface;

        std::unique_ptr<WinApiInterface> Bootstrap(HINSTANCE instance);
    } // namespace Windows
} // namespace UKControllerPlugin
