#pragma once
#include "prenote/AbstractPrenote.h"

namespace UKControllerPlugin {
    namespace Ownership {
        class AirfieldOwnershipManager;
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
    class AbstractPrenote;
    /*
        Handles the processing and sending of prenotes.
    */
    class PrenoteService
    {
        public:
        PrenoteService(
            const UKControllerPlugin::Ownership::AirfieldOwnershipManager& airfieldOwnership,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns,
            UKControllerPlugin::Message::UserMessager& userMessager);
        void AddPrenote(std::unique_ptr<const UKControllerPlugin::Prenote::AbstractPrenote> prenote);
        void CancelPrenote(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan);
        [[nodiscard]] auto CountPrenotes() const -> size_t;
        void SendPrenotes(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan);

        private:
        void PrenoteNotify(
            const UKControllerPlugin::Prenote::AbstractPrenote& prenote,
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan);

        // All the prenotes
        std::set<std::unique_ptr<const UKControllerPlugin::Prenote::AbstractPrenote>> prenotes;

        // Who's actively controlling
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;

        // Who owns what airfield
        const UKControllerPlugin::Ownership::AirfieldOwnershipManager& airfieldOwnership;

        // List of callsigns already prenoted
        std::set<std::string> alreadyPrenoted;

        // Allows us to send messagesto the user.
        UKControllerPlugin::Message::UserMessager& userMessager;
    };
} // namespace UKControllerPlugin::Prenote
