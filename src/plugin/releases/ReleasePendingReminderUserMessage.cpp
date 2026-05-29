#include "ReleasePendingReminderUserMessage.h"

namespace UKControllerPlugin::Releases {

    ReleasePendingReminderUserMessage::ReleasePendingReminderUserMessage(
        std::string callsign, std::string requestingController)
        : callsign(std::move(callsign)), requestingController(std::move(requestingController))
    {
    }

    auto ReleasePendingReminderUserMessage::MessageHandler() const -> std::string
    {
        return "DEPARTURE_RELEASE";
    }

    auto ReleasePendingReminderUserMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }

    auto ReleasePendingReminderUserMessage::MessageString() const -> std::string
    {
        return "Departure release for " + this->callsign + " from " + this->requestingController + " is still pending.";
    }

    auto ReleasePendingReminderUserMessage::MessageShowHandler() const -> bool
    {
        return true;
    }

    auto ReleasePendingReminderUserMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }

    auto ReleasePendingReminderUserMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }

    auto ReleasePendingReminderUserMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }

    auto ReleasePendingReminderUserMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::Releases
