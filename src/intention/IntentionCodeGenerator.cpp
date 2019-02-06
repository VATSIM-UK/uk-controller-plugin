#include "pch/stdafx.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/AirfieldGroup.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitRepository.h"
#include "intention/SectorExitPoint.h"

using UKControllerPlugin::IntentionCode::IntentionCodeData;
using UKControllerPlugin::IntentionCode::SectorExitPoint;
using UKControllerPlugin::IntentionCode::SectorExitRepository;
using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Set up the lists. This is hard coded for now, but may change.
        */
        IntentionCodeGenerator::IntentionCodeGenerator(
            std::vector<std::unique_ptr<AirfieldGroup>> airfieldGroups,
            SectorExitRepository & exitPoints
        )
            : airfieldGroups(std::move(airfieldGroups)), exitPoints(exitPoints)
        {

        }

        /*
            Looks through the route from start to finish for a sector exit point. Returns the index in the route where
            it may be found.
        */
        int IntentionCodeGenerator::FindFirExitPoint(EuroscopeExtractedRouteInterface & route)
        {
            int i = 0;
            while (i < route.GetPointsNumber()) {

                // If we find an exit point, return that, subject to rules;
                if (exitPoints.HasSectorExitPoint(route.GetPointName(i))) {

                    // Check that they're travelling in the right direction to leave the FIR at this point.
                    if (
                        i + 1 < route.GetPointsNumber() &&
                        !exitPoints.GetSectorExitPoint(route.GetPointName(i))
                            .IsCorrectOutDirection(
                                route.GetPointPosition(i).DirectionTo(route.GetPointPosition(i + 1))
                            )
                        ) {
                        i++;
                        continue;
                    }

                    // If they're cleared direct beyond their SEP, but not yet close to it
                    // then keep the intention code for now.
                    if (route.GetPointsAssignedIndex() > i && route.GetPointsCalculatedIndex() <= i) {
                        return i;
                    }

                    // Otherwise, if they're past the exit fix, they're not going to exit.
                    if (route.GetPointDistanceInMinutes(i) == this->exitPointPassed) {
                        return this->invalidExitPointIndex;
                    }

                    return i;
                }
                i++;
            }

            // Nothing found
            return this->invalidExitPointIndex;
        }

        /*
            Returns an intention code for the given flightplan. Starts with specific airfield edge cases and
            ends by checking route strings for sector exit points.
        */

        IntentionCodeData IntentionCodeGenerator::GetIntentionCodeForFlightplan(
            std::string callsign,
            std::string source,
            std::string destination,
            EuroscopeExtractedRouteInterface & route,
            int cruiseLevel
        ) {
            // No flightplan filed, so we'll kill this one here.
            if (source.compare("") == 0) {
                return IntentionCodeData(this->invalidCode, false, this->invalidExitPointIndex);
            }

            // Go through the airfield groups, to see if we can find a match.
            for (
                std::vector<std::unique_ptr<AirfieldGroup>>::const_iterator group = this->airfieldGroups.cbegin();
                group != this->airfieldGroups.cend();
                ++group
            ) {
                if ((*group)->HasAirfield(destination, route)) {

                    return IntentionCodeData(
                        (*group)->GetIntentionCodeForGroup(destination, route),
                        false,
                        this->invalidExitPointIndex
                    );
                }
            }

            // Look for a known sector exit fix
            int exitIndex = this->FindFirExitPoint(route);
            if (exitIndex != this->invalidExitPointIndex) {

                if (!this->exitPoints.HasSectorExitPoint(route.GetPointName(exitIndex))) {
                    LogError("Discovered invalid exit point " + std::string(route.GetPointName(exitIndex)));
                }

                return IntentionCodeData(
                    this->exitPoints.GetSectorExitPoint(
                        route.GetPointName(exitIndex)).GetIntentionCode(route, exitIndex, cruiseLevel),
                    true,
                    exitIndex
                );
            }

            // No special cases, just return the ICAO code.
            return IntentionCodeData(
                destination,
                false,
                this->invalidExitPointIndex
            );
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
