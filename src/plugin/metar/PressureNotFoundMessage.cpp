#include "PressureNotFoundMessage.h"

namespace UKControllerPlugin::Metar {

    PressureNotFoundMessage::PressureNotFoundMessage(std::string airfield) : airfield(airfield)
    {
    }

    auto PressureNotFoundMessage::MessageHandler() const -> std::string
    {
        return "UKCP_PRESSURE_MONITOR";
    }

    auto PressureNotFoundMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }

    auto PressureNotFoundMessage::MessageString() const -> std::string
    {
        return "Pressure information is not available for " + airfield + ".";
    }

    auto PressureNotFoundMessage::MessageShowHandler() const -> bool
    {
        return true;
    }

    auto PressureNotFoundMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }

    auto PressureNotFoundMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }

    auto PressureNotFoundMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }

    auto PressureNotFoundMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }
} // namespace UKControllerPlugin::Metar
