#include "PressureChangeMessage.h"
#include "PressureComponent.h"

namespace UKControllerPlugin::Metar {

    PressureChangeMessage::PressureChangeMessage(
        std::string airfield, const PressureComponent& pressureBefore, const PressureComponent& pressureNow)
        : airfield(airfield), pressureBefore(pressureBefore), pressureNow(pressureNow)
    {
    }

    auto PressureChangeMessage::MessageHandler() const -> std::string
    {
        return "UKCP_PRESSURE_MONITOR";
    }

    auto PressureChangeMessage::MessageSender() const -> std::string
    {
        return "UKCP";
    }

    auto PressureChangeMessage::MessageString() const -> std::string
    {
        return "Pressure change at " + airfield + ". QNH was " + this->QnhBefore() + ", now " + this->QnhAfter() +
               ". QFE was " + this->QfeBefore() + ", now " + this->QfeAfter() + ".";
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

    auto PressureChangeMessage::QfeBefore() const -> std::string
    {
        return this->pressureBefore.ReportedAsHectopascals() ? std::to_string(this->pressureBefore.QfeHectopascals())
                                                             : FormatInHg(this->pressureBefore.QfeInHg());
    }

    auto PressureChangeMessage::QfeAfter() const -> std::string
    {
        return this->pressureNow.ReportedAsHectopascals() ? std::to_string(this->pressureNow.QfeHectopascals())
                                                          : FormatInHg(this->pressureNow.QfeInHg());
    }

    auto PressureChangeMessage::QnhBefore() const -> std::string
    {
        return this->pressureBefore.ReportedAsHectopascals() ? std::to_string(this->pressureBefore.QnhHectopascals())
                                                             : FormatInHg(this->pressureBefore.QnhInHg());
    }

    auto PressureChangeMessage::QnhAfter() const -> std::string
    {
        return this->pressureNow.ReportedAsHectopascals() ? std::to_string(this->pressureNow.QnhHectopascals())
                                                          : FormatInHg(this->pressureNow.QnhInHg());
    }

    auto PressureChangeMessage::FormatInHg(float pressure) -> std::string
    {
        char formatted[INHG_BUFFER_SIZE];       // NOLINT
        sprintf_s(formatted, "%.2f", pressure); // NOLINT
        return formatted;
    }
} // namespace UKControllerPlugin::Metar
