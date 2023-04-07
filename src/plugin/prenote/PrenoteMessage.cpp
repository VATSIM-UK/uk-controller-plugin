#include "PrenoteMessage.h"
#include "controller/ControllerPosition.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPlugin::Prenote {
    PrenoteMessage::PrenoteMessage(
        int id,
        std::string callsign,
        std::string departureAirfield,
        std::string sid,
        std::string destinationAirfield,
        std::shared_ptr<Controller::ControllerPosition> sendingController,
        std::shared_ptr<Controller::ControllerPosition> targetController,
        std::chrono::system_clock::time_point expiresAt)
        : id(id), callsign(std::move(callsign)), departureAirfield(std::move(departureAirfield)), sid(std::move(sid)),
          destinationAirfield(std::move(destinationAirfield)), sendingController(sendingController),
          targetController(targetController), expiresAt(expiresAt),
          acknowledgedAt((std::chrono::system_clock::time_point::max)()), createdAt(Time::TimeNow())
    {
    }

    auto PrenoteMessage::GetId() const -> int
    {
        return id;
    }

    auto PrenoteMessage::GetCallsign() const -> const std::string&
    {
        return callsign;
    }

    auto PrenoteMessage::GetDepartureAirfield() const -> const std::string&
    {
        return departureAirfield;
    }

    auto PrenoteMessage::GetSid() const -> const std::string&
    {
        return sid;
    }

    auto PrenoteMessage::GetDestinationAirfield() const -> const std::string&
    {
        return destinationAirfield;
    }

    auto PrenoteMessage::GetSendingControllerId() const -> int
    {
        return sendingController->GetId();
    }

    auto PrenoteMessage::GetTargetControllerId() const -> int
    {
        return targetController->GetId();
    }

    auto PrenoteMessage::GetExpiresAt() const -> const std::chrono::system_clock::time_point&
    {
        return expiresAt;
    }

    auto PrenoteMessage::IsAcknowledged() const -> bool
    {
        return this->acknowledgedAt != (std::chrono::system_clock::time_point::max)();
    }

    void PrenoteMessage::Acknowledge()
    {
        this->acknowledgedAt = TimeNow();
    }

    auto PrenoteMessage::GetAcknowledgedAt() const -> const std::chrono::system_clock::time_point&
    {
        return this->acknowledgedAt;
    }

    auto PrenoteMessage::GetCreatedAt() const -> const std::chrono::system_clock::time_point&
    {
        return this->createdAt;
    }

    auto PrenoteMessage::GetSendingController() const -> std::shared_ptr<Controller::ControllerPosition>
    {
        return this->sendingController;
    }

    auto PrenoteMessage::GetTargetController() const -> std::shared_ptr<Controller::ControllerPosition>
    {
        return this->targetController;
    }
} // namespace UKControllerPlugin::Prenote
