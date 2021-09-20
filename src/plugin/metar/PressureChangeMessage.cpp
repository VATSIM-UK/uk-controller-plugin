#include "PressureChangeMessage.h"

namespace UKControllerPlugin::Metar {

    PressureChangeMessage::PressureChangeMessage(std::string station, std::string qnhBefore, std::string qnhAfter)
        : qnhBefore(std::move(qnhBefore)), qnhAfter(std::move(qnhAfter)), station(std::move(station))
    {
    }

    auto PressureChangeMessage::MessageHandler() const -> std::string
    {
        return "UKCP_QNH";
    }

    auto PressureChangeMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }

    auto PressureChangeMessage::MessageString() const -> std::string
    {
        return "New QNH at " + station + ", Was: " + qnhBefore + ", Now: " + qnhAfter;
    }

    auto PressureChangeMessage::MessageShowHandler() const -> bool
    {
        return true;
    }

    auto PressureChangeMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }

    auto PressureChangeMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }

    auto PressureChangeMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }

    auto PressureChangeMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::Metar
