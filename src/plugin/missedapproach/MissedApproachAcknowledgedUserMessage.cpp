#include "MissedApproach.h"
#include "MissedApproachAcknowledgedUserMessage.h"

namespace UKControllerPlugin::MissedApproach {

    MissedApproachAcknowledgedUserMessage::MissedApproachAcknowledgedUserMessage(const MissedApproach& approach)
        : approach(approach)
    {
    }

    auto MissedApproachAcknowledgedUserMessage::MessageHandler() const -> std::string
    {
        return "MISSED_APPROACH";
    }
    auto MissedApproachAcknowledgedUserMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }
    auto MissedApproachAcknowledgedUserMessage::MessageString() const -> std::string
    {
        std::string remarksString = this->approach.Remarks().empty() ? "" : " Remarks: " + this->approach.Remarks();
        return "Missed approach for " + this->approach.Callsign() + " acknowledged by " +
               this->approach.AcknowledgedBy() + "." + remarksString;
    }
    auto MissedApproachAcknowledgedUserMessage::MessageShowHandler() const -> bool
    {
        return true;
    }
    auto MissedApproachAcknowledgedUserMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }
    auto MissedApproachAcknowledgedUserMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }
    auto MissedApproachAcknowledgedUserMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }
    auto MissedApproachAcknowledgedUserMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::MissedApproach
