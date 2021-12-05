#include "MissedApproach.h"
#include "MissedApproachAcknowledgedUserMessage.h"

namespace UKControllerPlugin::MissedApproach {

    MissedApproachAcknowledgedUserMessage::MissedApproachAcknowledgedUserMessage(const MissedApproach& approach)
        : approach(approach)
    {
    }

    std::string MissedApproachAcknowledgedUserMessage::MessageHandler(void) const
    {
        return "MISSED_APPROACH";
    }
    std::string MissedApproachAcknowledgedUserMessage::MessageSender(void) const
    {
        return "UKCP";
    }
    std::string MissedApproachAcknowledgedUserMessage::MessageString(void) const
    {
        std::string remarksString = this->approach.Remarks().empty() ? "" : " Remarks: " + this->approach.Remarks();
        return "Missed approach for " + this->approach.Callsign() + " acknowledged by " +
               this->approach.AcknowledgedBy() + "." + remarksString;
    }
    bool MissedApproachAcknowledgedUserMessage::MessageShowHandler(void) const
    {
        return true;
    }
    bool MissedApproachAcknowledgedUserMessage::MessageMarkUnread(void) const
    {
        return true;
    }
    bool MissedApproachAcknowledgedUserMessage::MessageOverrideBusy(void) const
    {
        return true;
    }
    bool MissedApproachAcknowledgedUserMessage::MessageFlashHandler(void) const
    {
        return true;
    }
    bool MissedApproachAcknowledgedUserMessage::MessageRequiresConfirm(void) const
    {
        return true;
    }
} // namespace UKControllerPlugin::MissedApproach
