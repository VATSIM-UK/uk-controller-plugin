#include "pch/pch.h"
#include "prenote/PrenoteUserMessage.h"
#include "prenote/AbstractPrenote.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;


namespace UKControllerPlugin {
    namespace Prenote {

        const std::string PrenoteUserMessage::handler = "Prenote";
        const std::string PrenoteUserMessage::sender = "UKCP";

        PrenoteUserMessage::PrenoteUserMessage(
            const AbstractPrenote & prenote,
            const ActiveCallsign & activeCallsign,
            const EuroScopeCFlightPlanInterface & flightplan
        ) {
            this->message = "Prenote to " + activeCallsign.GetCallsign() + " required for " +
                flightplan.GetCallsign() + " (" + prenote.GetSummaryString() + ")";
        }

        std::string PrenoteUserMessage::MessageHandler(void) const
        {
            return this->handler;
        }

        std::string PrenoteUserMessage::MessageSender(void) const
        {
            return this->sender;
        }

        std::string PrenoteUserMessage::MessageString(void) const
        {
            return this->message;
        }

        bool PrenoteUserMessage::MessageShowHandler(void) const
        {
            return true;
        }

        bool PrenoteUserMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        bool PrenoteUserMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        bool PrenoteUserMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        bool PrenoteUserMessage::MessageRequiresConfirm(void) const
        {
            return true;
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
