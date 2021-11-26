#include "PrenoteUserMessage.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin::Prenote {

    PrenoteUserMessage::PrenoteUserMessage(
        const ActiveCallsign& activeCallsign, const EuroScopeCFlightPlanInterface& flightplan)
    {
        this->message = "Prenote to " + activeCallsign.GetCallsign() + " required for " + flightplan.GetCallsign() +
                        " (" + GetSummaryString(flightplan) + ")";
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

    auto PrenoteUserMessage::GetSummaryString(const EuroScopeCFlightPlanInterface& flightplan) -> std::string
    {
        return flightplan.GetSidName().empty() ? flightplan.GetOrigin()
                                               : flightplan.GetOrigin() + "/" + flightplan.GetSidName();
    }
} // namespace UKControllerPlugin::Prenote
