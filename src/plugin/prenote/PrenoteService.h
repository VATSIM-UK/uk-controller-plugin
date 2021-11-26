#pragma once

namespace UKControllerPlugin {
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership

    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller

    namespace Message {
        class UserMessager;
    } // namespace Message

    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class PublishedPrenoteMapper;

    /*
        Handles the processing and sending of prenotes.
    */
    class PrenoteService
    {
        public:
        PrenoteService(
            const PublishedPrenoteMapper& prenoteMapper,
            const UKControllerPlugin::Ownership::AirfieldServiceProviderCollection& airfieldOwnership,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns,
            UKControllerPlugin::Message::UserMessager& userMessager);
        ~PrenoteService();
        PrenoteService(const PrenoteService&) = delete;
        PrenoteService(PrenoteService&&) noexcept;
        auto operator=(const PrenoteService&) -> PrenoteService& = delete;
        auto operator=(PrenoteService&&) noexcept -> PrenoteService& = delete;
        void CancelPrenote(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan);
        void SendPrenotes(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan);

        private:
        // Maps flightplan to prenote
        const PublishedPrenoteMapper& prenoteMapper;

        // Who's actively controlling
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;

        // Who owns what airfield
        const UKControllerPlugin::Ownership::AirfieldServiceProviderCollection& airfieldOwnership;

        // List of callsigns already prenoted
        std::set<std::string> alreadyPrenoted;

        // Allows us to send messages to the user.
        UKControllerPlugin::Message::UserMessager& userMessager;
    };
} // namespace UKControllerPlugin::Prenote
