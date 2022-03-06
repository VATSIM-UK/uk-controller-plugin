#pragma once
#include "aircraft/CallsignSelectionProviderInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope

    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Wake {
    class WakeCalculatorOptions;

    /*
     * Provides the lead callsign list for the wake calculator.
     */
    class LeadWakeCallsignProvider : public Aircraft::CallsignSelectionProviderInterface
    {
        public:
        LeadWakeCallsignProvider(
            const Ownership::AirfieldServiceProviderCollection& serviceProvisions,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            std::shared_ptr<WakeCalculatorOptions> options);
        auto GetCallsigns() -> std::set<std::string> override;
        void CallsignSelected(const std::string& callsign) override;

        private:
        [[nodiscard]] auto
        RelevantAirfieldForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> std::string;

        // Whos providing what, where
        const Ownership::AirfieldServiceProviderCollection& serviceProvisions;

        // Plugin for looping flightplans
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The options for the calculator
        std::shared_ptr<WakeCalculatorOptions> options;
    };
} // namespace UKControllerPlugin::Wake
