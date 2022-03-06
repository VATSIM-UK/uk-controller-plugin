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
        Looks through the route from start to finish for a sector exit point.

        Returns a pair of indexes in the parsed route - the first is what generated the intention code, the second,
        which may be the same, is the point at which the aircraft exits UK airspace and thus is useful for things
        like SRD searches.
    */
    auto IntentionCodeGenerator::FindFirExitPoint(EuroscopeExtractedRouteInterface& route) const -> std::pair<int, int>
    {
        int i = 0;
        std::pair<int, int> exitIndexes = invalidExitPair;
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

                this->SetExitIndexes(exitIndexes, i);
                if (this->FoundAllExitPoints(exitIndexes)) {
                    return exitIndexes;
                }
            }
            i++;
        }

        // At this point, we've either found nothing, or the first exit fix found is also the UK area exit fix too.
        return exitIndexes;
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
        int cruiseLevel) const -> IntentionCodeData
    {
        // No flightplan filed, so we'll kill this one here.
        if (source.empty()) {
            return {
                this->invalidCode,
                IntentionCodeData::INVALID_EXIT_POINT,
                IntentionCodeData::INVALID_EXIT_POINT,
                "",
                ""};
        }

        // Go through the airfield groups, to see if we can find a match.
        for (auto group = this->airfieldGroups.cbegin(); group != this->airfieldGroups.cend(); ++group) {
            if ((*group)->HasAirfield(destination, route) &&
                (*group)->AppliesToController(this->userControllerPosition, route)) {

                return {
                    (*group)->GetIntentionCodeForGroup(destination, route),
                    IntentionCodeData::INVALID_EXIT_POINT,
                    IntentionCodeData::INVALID_EXIT_POINT,
                    "",
                    ""};
            }
        }

        // Look for a known sector exit fix
        auto exitIndexes = this->FindFirExitPoint(route);
        if (exitIndexes != invalidExitPair) {

            if (!this->exitPoints.HasSectorExitPoint(route.GetPointName(exitIndexes.first))) {
                LogError("Discovered invalid first exit point " + std::string(route.GetPointName(exitIndexes.first)));
                // Just return the ICAO code.
                return {
                    destination, IntentionCodeData::INVALID_EXIT_POINT, IntentionCodeData::INVALID_EXIT_POINT, "", ""};
            }

            if (!this->exitPoints.HasSectorExitPoint(route.GetPointName(exitIndexes.second))) {
                LogError("Discovered invalid second exit point " + std::string(route.GetPointName(exitIndexes.second)));
                // Just return the ICAO code.
                return {
                    destination, IntentionCodeData::INVALID_EXIT_POINT, IntentionCodeData::INVALID_EXIT_POINT, "", ""};
            }

            return {
                this->exitPoints.GetSectorExitPoint(route.GetPointName(exitIndexes.first))
                    .GetIntentionCode(route, exitIndexes.first, cruiseLevel),
                exitIndexes.first,
                exitIndexes.second,
                route.GetPointName(exitIndexes.first),
                route.GetPointName(exitIndexes.second)};
        }

        // No special cases, just return the ICAO code.
        return {destination, IntentionCodeData::INVALID_EXIT_POINT, IntentionCodeData::INVALID_EXIT_POINT, "", ""};
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

    void IntentionCodeGenerator::SetExitIndexes(std::pair<int, int>& indexes, int index) const
    {
        if (indexes.first == this->invalidExitPointIndex) {
            indexes.first = index;
            indexes.second = index;
        } else {
            indexes.second = index;
        }
    }

    /**
     * If we've found a first and second exit point that are different, we've found all we need.
     */
    auto IntentionCodeGenerator::FoundAllExitPoints(const std::pair<int, int>& indexes) const -> bool
    {
        return indexes.first != this->invalidExitPointIndex && indexes.second != this->invalidExitPointIndex &&
               indexes.second != indexes.first;
    }
} // namespace UKControllerPlugin::IntentionCode
