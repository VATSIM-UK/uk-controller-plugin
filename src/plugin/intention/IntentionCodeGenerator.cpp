#include "AirfieldGroup.h"
#include "IntentionCodeGenerator.h"
#include "SectorExitPoint.h"
#include "SectorExitRepository.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;
using UKControllerPlugin::IntentionCode::IntentionCodeData;
using UKControllerPlugin::IntentionCode::SectorExitPoint;
using UKControllerPlugin::IntentionCode::SectorExitRepository;

namespace UKControllerPlugin::IntentionCode {

    /*
        Set up the lists. This is hard coded for now, but may change.
    */
    IntentionCodeGenerator::IntentionCodeGenerator(
        std::vector<std::unique_ptr<AirfieldGroup>> airfieldGroups, SectorExitRepository& exitPoints)
        : airfieldGroups(std::move(airfieldGroups)), exitPoints(exitPoints)
    {
    }

    IntentionCodeGenerator::~IntentionCodeGenerator() = default;
    IntentionCodeGenerator::IntentionCodeGenerator(IntentionCodeGenerator&&) noexcept = default;

    /*
        Looks through the route from start to finish for a sector exit point. Returns the index in the route where
        it may be found.
    */
    auto IntentionCodeGenerator::FindFirExitPoint(EuroscopeExtractedRouteInterface& route) -> int
    {
        int i = 0;
        while (i < route.GetPointsNumber()) {

            // If we find an exit point, return that, subject to rules;
            if (exitPoints.HasSectorExitPoint(route.GetPointName(i))) {

                // Check that they're travelling in the right direction to leave the FIR at this point.
                // Also make sure they're on the right position to use the exit point.
                const SectorExitPoint& point = exitPoints.GetSectorExitPoint(route.GetPointName(i));
                if (i + 1 < route.GetPointsNumber() &&
                    (!point.IsCorrectOutDirection(
                         route.GetPointPosition(i).DirectionTo(route.GetPointPosition(i + 1))) ||
                     !point.AppliesToController(this->userControllerPosition))) {
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
                    return invalidExitPointIndex;
                }

                return i;
            }
            i++;
        }

        // Nothing found
        return invalidExitPointIndex;
    }

    /*
        Returns an intention code for the given flightplan. Starts with specific airfield edge cases and
        ends by checking route strings for sector exit points.
    */

    auto IntentionCodeGenerator::GetIntentionCodeForFlightplan(
        const std::string& callsign,
        const std::string& source,
        const std::string& destination,
        EuroscopeExtractedRouteInterface& route,
        int cruiseLevel) -> IntentionCodeData
    {
        // No flightplan filed, so we'll kill this one here.
        if (source.empty()) {
            return {this->invalidCode, false, invalidExitPointIndex, ""};
        }

        // Go through the airfield groups, to see if we can find a match.
        for (auto group = this->airfieldGroups.cbegin(); group != this->airfieldGroups.cend(); ++group) {
            if ((*group)->HasAirfield(destination, route) &&
                (*group)->AppliesToController(this->userControllerPosition, route)) {

                return {(*group)->GetIntentionCodeForGroup(destination, route), false, invalidExitPointIndex, ""};
            }
        }

        // Look for a known sector exit fix
        int exitIndex = this->FindFirExitPoint(route);
        if (exitIndex != invalidExitPointIndex) {

            if (!this->exitPoints.HasSectorExitPoint(route.GetPointName(exitIndex))) {
                LogError("Discovered invalid exit point " + std::string(route.GetPointName(exitIndex)));
                // Just return the ICAO code.
                return {destination, false, invalidExitPointIndex, ""};
            }

            return {
                this->exitPoints.GetSectorExitPoint(route.GetPointName(exitIndex))
                    .GetIntentionCode(route, exitIndex, cruiseLevel),
                true,
                exitIndex,
                route.GetPointName(exitIndex)};
        }

        // No special cases, just return the ICAO code.
        return {destination, false, invalidExitPointIndex, ""};
    }

    void IntentionCodeGenerator::SetUserControllerPosition(std::string position)
    {
        this->userControllerPosition = std::move(position);
    }

    auto IntentionCodeGenerator::GetUserControllerPosition() const -> std::string
    {
        return this->userControllerPosition;
    }

    auto IntentionCodeGenerator::GetInvalidExitPointIndex() -> int
    {
        return invalidExitPointIndex;
    }
} // namespace UKControllerPlugin::IntentionCode
