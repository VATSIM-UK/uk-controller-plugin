#include "pch/pch.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/AbstractPrenote.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;


namespace UKControllerPlugin {
    namespace Prenote {

        const std::string PrenoteMessage::handler = "Prenote";
        const std::string PrenoteMessage::sender = "UKCP";

        PrenoteMessage::PrenoteMessage(
            const AbstractPrenote & prenote,
            const ActiveCallsign & activeCallsign,
            const EuroScopeCFlightPlanInterface & flightplan
        ) {
            this->message = "Prenote to " + activeCallsign.GetCallsign() + " required for " +
                flightplan.GetCallsign() + " (" + prenote.GetSummaryString() + ")";
        }

        std::string PrenoteMessage::MessageHandler(void) const
        {
            return this->handler;
        }

        std::string PrenoteMessage::MessageSender(void) const
        {
            return this->sender;
        }

        std::string PrenoteMessage::MessageString(void) const
        {
            return this->message;
        }

        bool PrenoteMessage::MessageShowHandler(void) const
        {
            return true;
        }

        bool PrenoteMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        bool PrenoteMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        bool PrenoteMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        bool PrenoteMessage::MessageRequiresConfirm(void) const
        {
            return true;
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
