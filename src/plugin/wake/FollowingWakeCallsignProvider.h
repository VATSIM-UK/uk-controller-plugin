#pragma once
#include "aircraft/CallsignSelectionProviderInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Wake {
    class WakeCalculatorOptions;

    /*
     * Provides the following callsign list for the wake calculator.
     */
    class FollowingWakeCallsignProvider : public Aircraft::CallsignSelectionProviderInterface
    {
        public:
        FollowingWakeCallsignProvider(
            Euroscope::EuroscopePluginLoopbackInterface& plugin, std::shared_ptr<WakeCalculatorOptions> options);
        auto GetCallsigns() -> std::set<std::string> override;
        void CallsignSelected(const std::string& callsign) override;

        private:
        // Plugin for looping flightplans
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The options for the calculator
        std::shared_ptr<WakeCalculatorOptions> options;
    };
} // namespace UKControllerPlugin::Wake
