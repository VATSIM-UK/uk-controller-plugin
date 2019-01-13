#include "pch/stdafx.h"
#include "controller/ControllerPosition.h"

namespace UKControllerPlugin {
    namespace Controller {
        ControllerPosition::ControllerPosition(
            std::string callsign,
            double frequency,
            std::string type,
            std::vector<std::string> topdown
        ) {
            this->callsign = callsign;
            this->frequency = frequency;
            this->type = type;
            this->topdown = topdown;
        }

        std::string ControllerPosition::GetUnit(void) const
        {
            return this->callsign.substr(0, this->callsign.find('_'));
        }

        std::string ControllerPosition::GetCallsign(void) const
        {
            return this->callsign;
        }

        double ControllerPosition::GetFrequency(void) const
        {
            return this->frequency;
        }

        std::string ControllerPosition::GetType(void) const
        {
            return this->type;
        }

        std::vector<std::string> ControllerPosition::GetTopdown(void) const
        {
            return this->topdown;
        }

        bool ControllerPosition::operator==(const ControllerPosition & position) const
        {
            return fabs(this->frequency - position.GetFrequency()) < 0.001 &&
                this->callsign.compare(position.GetCallsign()) == 0 &&
                this->type.compare(position.GetType()) == 0;
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
