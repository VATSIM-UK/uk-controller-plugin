#pragma once

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Class for validating squawks either returned from the API or manually
            inserted by the user.
        */
        class SquawkValidator
        {
            public:
                bool static AllowedSquawk(std::string squawk);
                bool static ValidSquawk(std::string squawk);
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
