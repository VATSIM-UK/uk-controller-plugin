#include "pch/pch.h"

#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "prenote/AbstractPrenote.h"
#include "prenote/PrenoteUserMessage.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Prenote::AbstractPrenote;

namespace UKControllerPlugin::Prenote {

    PrenoteUserMessage::PrenoteUserMessage(
        const AbstractPrenote& prenote,
        const ActiveCallsign& activeCallsign,
        const EuroScopeCFlightPlanInterface& flightplan)
    {
        this->message = "Prenote to " + activeCallsign.GetCallsign() + " required for " + flightplan.GetCallsign() +
                        " (" + prenote.GetSummaryString() + ")";
    }

    auto PrenoteUserMessage::MessageHandler() const -> std::string
    {
        return this->handler;
    }

    auto PrenoteUserMessage::MessageSender() const -> std::string
    {
        return this->sender;
    }

    auto PrenoteUserMessage::MessageString() const -> std::string
    {
        return this->message;
    }

    auto PrenoteUserMessage::MessageShowHandler() const -> bool
    {
        return true;
    }

    auto PrenoteUserMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }

    auto PrenoteUserMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }

    auto PrenoteUserMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }

    auto PrenoteUserMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::Prenote
