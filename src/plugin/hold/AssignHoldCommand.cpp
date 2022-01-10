#include "AssignHoldCommand.h"
#include "HoldManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

namespace UKControllerPlugin::Hold {

    AssignHoldCommand::AssignHoldCommand(HoldManager& holdManager, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : holdManager(holdManager), plugin(plugin), noholdRegex("^\\.ukcp nohold( ([A-Za-z0-9\\-_]+))?$"),
          holdRegex("^\\.ukcp hold ([A-Za-z]+)( ([A-Za-z0-9\\-_]+))?$")
    {
    }

    auto AssignHoldCommand::ProcessCommand(std::string command) -> bool
    {
        std::smatch regexMatches;
        if (std::regex_match(command, regexMatches, noholdRegex)) {
            std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> flightplan =
                regexMatches[2].matched ? plugin.GetFlightplanForCallsign(regexMatches[2].str())
                                        : plugin.GetSelectedFlightplan();

            if (!flightplan) {
                return false;
            }

            holdManager.UnassignAircraftFromHold(flightplan->GetCallsign(), true);
            return true;
        }

        if (std::regex_match(command, regexMatches, holdRegex)) {

            std::shared_ptr<Euroscope::EuroScopeCFlightPlanInterface> flightplan =
                regexMatches[3].matched ? plugin.GetFlightplanForCallsign(regexMatches[3].str())
                                        : plugin.GetSelectedFlightplan();

            if (!flightplan) {
                return false;
            }

            holdManager.AssignAircraftToHold(flightplan->GetCallsign(), regexMatches[1].str(), true);
            return true;
        }

        return false;
    }
} // namespace UKControllerPlugin::Hold
