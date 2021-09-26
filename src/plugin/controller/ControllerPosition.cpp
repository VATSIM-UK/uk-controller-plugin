#include "ControllerPosition.h"

namespace UKControllerPlugin::Controller {
    ControllerPosition::ControllerPosition(
        int id,
        std::string callsign,
        double frequency,
        std::vector<std::string> topdown,
        bool requestsDepartureReleases,
        bool receivesDepartureReleases,
        bool sendsPrenoteMessages,
        bool receivesPrenoteMessages)
        : id(id), callsign(std::move(callsign)), frequency(frequency), topdown(std::move(topdown)),
          requestsDepartureReleases(requestsDepartureReleases), receivesDepartureReleases(receivesDepartureReleases),
          sendsPrenoteMessages(sendsPrenoteMessages), receivesPrenoteMessages(receivesPrenoteMessages)
    {
    }

    auto ControllerPosition::GetId() const -> int
    {
        return this->id;
    }

    auto ControllerPosition::GetUnit() const -> std::string
    {
        return this->callsign.substr(0, this->callsign.find('_'));
    }

    auto ControllerPosition::GetCallsign() const -> std::string
    {
        return this->callsign;
    }

    auto ControllerPosition::GetFrequency() const -> double
    {
        return this->frequency;
    }

    auto ControllerPosition::GetType() const -> std::string
    {
        return callsign.substr(this->callsign.size() - 3, this->callsign.size());
    }

    auto ControllerPosition::HasTopdownAirfield(const std::string& icao) const -> bool
    {
        return std::find(this->topdown.cbegin(), this->topdown.cend(), icao) != this->topdown.cend();
    }

    auto ControllerPosition::RequestsDepartureReleases() const -> bool
    {
        return this->requestsDepartureReleases;
    }

    auto ControllerPosition::ReceivesDepartureReleases() const -> bool
    {
        return this->receivesDepartureReleases;
    }

    auto ControllerPosition::GetTopdown() const -> std::vector<std::string>
    {
        return this->topdown;
    }

    auto ControllerPosition::operator==(const ControllerPosition& position) const -> bool
    {
        return this->GetId() == position.GetId() &&
               fabs(this->frequency - position.GetFrequency()) < frequencyMatchDelta &&
               this->callsign == position.GetCallsign();
    }

    auto ControllerPosition::SendsPrenoteMessages() const -> bool
    {
        return this->sendsPrenoteMessages;
    }

    auto ControllerPosition::ReceivesPrenoteMessages() const -> bool
    {
        return this->receivesPrenoteMessages;
    }

    auto ControllerPosition::IsTower() const -> bool
    {
        return this->GetType() == "TWR";
    }
} // namespace UKControllerPlugin::Controller
