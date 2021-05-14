#include "pch/stdafx.h"
#include "controller/ControllerPosition.h"

namespace UKControllerPlugin {
    namespace Controller {
        ControllerPosition::ControllerPosition(
            int id,
            std::string callsign,
            double frequency,
            std::vector<std::string> topdown,
            bool requestsDepartureReleases,
            bool receivesDepartureReleases
        ): id(id), requestsDepartureReleases(requestsDepartureReleases),
           receivesDepartureReleases(receivesDepartureReleases), callsign(callsign),
           frequency(frequency), topdown(std::move(topdown))
        { }

        int ControllerPosition::GetId() const
        {
            return this->id;
        }

        std::string ControllerPosition::GetUnit() const
        {
            return this->callsign.substr(0, this->callsign.find('_'));
        }

        std::string ControllerPosition::GetCallsign() const
        {
            return this->callsign;
        }

        double ControllerPosition::GetFrequency() const
        {
            return this->frequency;
        }

        std::string ControllerPosition::GetType() const
        {
            return callsign.substr(this->callsign.size() - 3, this->callsign.size());
        }

        bool ControllerPosition::HasTopdownAirfield(std::string icao) const
        {
            return std::find(
                this->topdown.cbegin(),
                this->topdown.cend(),
                icao
            ) != this->topdown.cend();
        }

        bool ControllerPosition::RequestsDepartureReleases() const
        {
            return this->requestsDepartureReleases;
        }

        bool ControllerPosition::ReceivesDepartureReleases() const
        {
            return this->receivesDepartureReleases;
        }

        std::vector<std::string> ControllerPosition::GetTopdown() const
        {
            return this->topdown;
        }

        bool ControllerPosition::operator==(const ControllerPosition& position) const
        {
            return this->GetId() == position.GetId() &&
                fabs(this->frequency - position.GetFrequency()) < 0.001 &&
                this->callsign.compare(position.GetCallsign()) == 0;
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
