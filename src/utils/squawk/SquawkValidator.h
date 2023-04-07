#pragma once

namespace UKControllerPlugin::Squawk {

    /*
        Class for validating squawks either returned from the API or manually
        inserted by the user.
    */
    class SquawkValidator
    {
        public:
        [[nodiscard]] auto static AllowedSquawk(const std::string& squawk) -> bool;
        [[nodiscard]] auto static ValidSquawk(std::string squawk) -> bool;
    };
} // namespace UKControllerPlugin::Squawk
