#include "ReleaseRejectionRemarksUserMessage.h"

namespace UKControllerPlugin::Releases {

    ReleaseRejectionRemarksUserMessage::ReleaseRejectionRemarksUserMessage(
        const std::string callsign, const std::string controller, const std::string remarks)
        : callsign(callsign), controller(controller), remarks(remarks)
    {
    }

    auto ReleaseRejectionRemarksUserMessage::MessageHandler() const -> std::string
    {
        return "DEPARTURE_RELEASE";
    }
    auto ReleaseRejectionRemarksUserMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }
    auto ReleaseRejectionRemarksUserMessage::MessageString() const -> std::string
    {
        return "Departure release rejection remarks by " + controller + " for " + callsign + ": " + remarks;
    }
    auto ReleaseRejectionRemarksUserMessage::MessageShowHandler() const -> bool
    {
        return true;
    }
    auto ReleaseRejectionRemarksUserMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }
    auto ReleaseRejectionRemarksUserMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }
    auto ReleaseRejectionRemarksUserMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }
    auto ReleaseRejectionRemarksUserMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::Releases
