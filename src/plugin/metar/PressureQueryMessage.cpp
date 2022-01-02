#include "PressureQueryMessage.h"
#include "PressureComponent.h"

namespace UKControllerPlugin::Metar {

    PressureQueryMessage::PressureQueryMessage(std::string airfield, const PressureComponent& pressure)
        : airfield(airfield), pressure(pressure)
    {
    }

    auto PressureQueryMessage::MessageHandler() const -> std::string
    {
        return "UKCP_PRESSURE_MONITOR";
    }

    auto PressureQueryMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }

    auto PressureQueryMessage::MessageString() const -> std::string
    {
        return "QNH at " + airfield + " is " + this->Qnh() + ", QFE is " + this->Qfe() + ".";
    }

    auto PressureQueryMessage::MessageShowHandler() const -> bool
    {
        return true;
    }

    auto PressureQueryMessage::MessageMarkUnread() const -> bool
    {
        return true;
    }

    auto PressureQueryMessage::MessageOverrideBusy() const -> bool
    {
        return true;
    }

    auto PressureQueryMessage::MessageFlashHandler() const -> bool
    {
        return true;
    }

    auto PressureQueryMessage::MessageRequiresConfirm() const -> bool
    {
        return true;
    }

    auto PressureQueryMessage::Qfe() const -> std::string
    {
        return this->pressure.ReportedAsHectopascals() ? std::to_string(this->pressure.QfeHectopascals())
                                                       : FormatInHg(this->pressure.QfeInHg());
    }

    auto PressureQueryMessage::Qnh() const -> std::string
    {
        return this->pressure.ReportedAsHectopascals() ? std::to_string(this->pressure.QnhHectopascals())
                                                       : FormatInHg(this->pressure.QnhInHg());
    }

    auto PressureQueryMessage::FormatInHg(float pressure) -> std::string
    {
        char formatted[INHG_BUFFER_SIZE];       // NOLINT
        sprintf_s(formatted, "%.2f", pressure); // NOLINT
        return formatted;
    }
} // namespace UKControllerPlugin::Metar
