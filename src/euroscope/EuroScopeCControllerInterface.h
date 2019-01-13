#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An interface for wrappers around the Euroscope CController
            class.
        */
        class EuroScopeCControllerInterface
        {
            public:
                virtual ~EuroScopeCControllerInterface(void) {}
                virtual const std::string GetCallsign(void) const = 0;
                virtual const double GetFrequency(void) const = 0;
                virtual const std::string GetControllerName(void) const = 0;
                virtual const bool HasActiveFrequency(void) const = 0;
                virtual const bool IsCurrentUser(void) const = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
