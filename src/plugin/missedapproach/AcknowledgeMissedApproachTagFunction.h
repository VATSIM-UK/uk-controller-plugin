#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    class MissedApproach;
    class MissedApproachCollection;

    class AcknowledgeMissedApproachTagFunction
    {
        public:
        AcknowledgeMissedApproachTagFunction(
            const MissedApproachCollection& missedApproaches,
            const Dialog::DialogManager& dialogManager,
            const Api::ApiInterface& api,
            const Ownership::AirfieldServiceProviderCollection& airfieldServiceProviders);
        void TriggerDialog(const Euroscope::EuroScopeCFlightPlanInterface& flightplan);
        void Acknowledge(const std::string& remarks) const;

        private:
        [[nodiscard]] auto MissedApproachForCallsign() const -> const std::shared_ptr<class MissedApproach>;
        [[nodiscard]] auto UserCanTrigger() const -> bool;

        std::string lastCallsign;
        std::string lastAirfield;

        // All the missed approaches
        const MissedApproachCollection& missedApproaches;

        // Dialog manager for the acknowledge dialog
        const Dialog::DialogManager& dialogManager;

        // API connection
        const Api::ApiInterface& api;

        // Who's providing services at an airfield
        const Ownership::AirfieldServiceProviderCollection& airfieldServiceProviders;
    };
} // namespace UKControllerPlugin::MissedApproach
