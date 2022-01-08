#include "MetarComponents.h"
#include "ParsedMetar.h"
#include "ParsedMetarCollection.h"
#include "PressureComponent.h"
#include "PressureNotFoundMessage.h"
#include "PressureQueryCommandHandler.h"
#include "PressureQueryMessage.h"
#include "message/UserMessager.h"

namespace UKControllerPlugin::Metar {

    PressureQueryCommandHandler::PressureQueryCommandHandler(
        const ParsedMetarCollection& metars, Message::UserMessager& userMessager)
        : metars(metars), userMessager(userMessager), commandPattern("^\\.ukcp pressure ([A-Z]{4})$")
    {
    }

    auto PressureQueryCommandHandler::ProcessCommand(std::string command) -> bool
    {
        std::smatch matches;
        if (!std::regex_search(command, matches, commandPattern)) {
            return false;
        }

        const std::string airfield = matches[1];
        const auto metar = this->metars.GetForAirfield(airfield);
        if (metar == nullptr || metar->Components().pressure == nullptr) {
            this->userMessager.SendMessageToUser(PressureNotFoundMessage(airfield));
        } else {
            this->userMessager.SendMessageToUser(PressureQueryMessage(airfield, *metar->Components().pressure));
        }

        return true;
    }
} // namespace UKControllerPlugin::Metar
