#pragma once
#include "aircraft/CallsignSelectionProviderInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Navaids {
        struct Navaid;
    } // namespace Navaids
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Hold {
    class HoldManager;

    /**
     * Provides callsigns to the callsign selection list
     * to add an aircraft to the hold.
     */
    class AddToHoldCallsignProvider : public Aircraft::CallsignSelectionProviderInterface
    {
        public:
        AddToHoldCallsignProvider(
            const Navaids::Navaid& navaid, HoldManager& holdManager, Euroscope::EuroscopePluginLoopbackInterface&
                                                                        plugin);
        auto GetCallsigns() -> std::set<std::string> override;
        void CallsignSelected(const std::string& callsign) override;

        private:
        // The holding navaid this is for
        const Navaids::Navaid& navaid;

        // For adding aircraft to holds
        HoldManager& holdManager;

        // For checking flightplans exist
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Hold
