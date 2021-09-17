#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsign;
    } // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class AbstractPrenote;

    /*
        A prenote message - follows the MessageSerializable interface
        to display a message to the user.
    */
    class PrenoteUserMessage : public UKControllerPlugin::Message::MessageSerializableInterface
    {
        public:
        PrenoteUserMessage(
            const UKControllerPlugin::Prenote::AbstractPrenote& prenote,
            const UKControllerPlugin::Controller::ActiveCallsign& activeCallsign,
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan);

        // Inherited via MessageSerializableInterface
        [[nodiscard]] auto MessageHandler() const -> std::string override;
        [[nodiscard]] auto MessageSender() const -> std::string override;
        [[nodiscard]] auto MessageString() const -> std::string override;
        [[nodiscard]] auto MessageShowHandler() const -> bool override;
        [[nodiscard]] auto MessageMarkUnread() const -> bool override;
        [[nodiscard]] auto MessageOverrideBusy() const -> bool override;
        [[nodiscard]] auto MessageFlashHandler() const -> bool override;
        [[nodiscard]] auto MessageRequiresConfirm() const -> bool override;

        private:
        // The message to send
        std::string message;

        // Handler to handle this message
        const std::string handler = "Prenote";

        // Who this message comes from
        const std::string sender = "UKCP";
    };
} // namespace UKControllerPlugin::Prenote
