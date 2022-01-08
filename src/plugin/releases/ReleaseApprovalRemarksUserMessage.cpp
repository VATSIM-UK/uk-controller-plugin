#include "ReleaseApprovalRemarksUserMessage.h"

namespace UKControllerPlugin::Releases {

    ReleaseApprovalRemarksUserMessage::ReleaseApprovalRemarksUserMessage(
        const std::string callsign, const std::string controller, const std::string remarks)
        : callsign(callsign), controller(controller), remarks(remarks)
    {
    }

    auto ReleaseApprovalRemarksUserMessage::MessageHandler() const -> std::string
    {
        return "DEPARTURE_RELEASE";
    }
    auto ReleaseApprovalRemarksUserMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }
    auto ReleaseApprovalRemarksUserMessage::MessageString() const -> std::string
    {
        return "Departure release approval remarks by " + controller + " for " + callsign + ": " + remarks;
    }
    auto ReleaseApprovalRemarksUserMessage::MessageShowHandler() const -> bool
    {
        return true;
    }
    auto ReleaseApprovalRemarksUserMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }
    auto ReleaseApprovalRemarksUserMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }
    auto ReleaseApprovalRemarksUserMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }
    auto ReleaseApprovalRemarksUserMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::Releases
