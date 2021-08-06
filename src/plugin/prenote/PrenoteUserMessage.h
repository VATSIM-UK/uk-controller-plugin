#pragma once
#include "message/MessageSerializableInterface.h"

namespace UKControllerPlugin {
    namespace Prenote {
        class AbstractPrenote;
    }  // namespace Prenote
    namespace Controller {
        class ActiveCallsign;
    }  // namespace Controller
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Prenote {

        /*
            A prenote message - follows the MessageSerializable interface
            to display a message to the user.
        */
        class PrenoteUserMessage : public UKControllerPlugin::Message::MessageSerializableInterface
        {
            public:
                PrenoteUserMessage(
                    const UKControllerPlugin::Prenote::AbstractPrenote & prenote,
                    const UKControllerPlugin::Controller::ActiveCallsign & activeCallsign,
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                );

                // Handler to handle this message
                static const std::string handler;

                // Who this message comes from
                static const std::string sender;

                // Inherited via MessageSerializableInterface
                std::string MessageHandler(void) const override;
                std::string MessageSender(void) const override;
                std::string MessageString(void) const override;
                bool MessageShowHandler(void) const override;
                bool MessageMarkUnread(void) const override;
                bool MessageOverrideBusy(void) const override;
                bool MessageFlashHandler(void) const override;
                bool MessageRequiresConfirm(void) const override;

            private:

                // The message to send
                std::string message;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
