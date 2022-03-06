#pragma once
#include "IntentionCodeData.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {
    class AirfieldGroup;
    class SectorExitRepository;

    /*
        Class that determines an intention code, given a flightplan.

        Only one publically facing method, which uses the private methods to determine
        what intention code to return. Once a flightplan has been recieved once, the
        result is cached so it doesn't have to be caclulated on every TAG load.
    */
    class IntentionCodeGenerator
    {
        public:
        IntentionCodeGenerator(
            std::vector<std::unique_ptr<UKControllerPlugin::IntentionCode::AirfieldGroup>> airfieldGroups,
            UKControllerPlugin::IntentionCode::SectorExitRepository& exitPoints);
        ~IntentionCodeGenerator();
        IntentionCodeGenerator(const IntentionCodeGenerator&) = delete;
        IntentionCodeGenerator(IntentionCodeGenerator&&) noexcept;
        auto operator=(const IntentionCodeGenerator&) -> IntentionCodeGenerator& = delete;
        auto operator=(IntentionCodeGenerator&&) -> IntentionCodeGenerator& = delete;

        auto GetIntentionCodeForFlightplan(
            const std::string& callsign,
            const std::string& origin,
            const std::string& destination,
            UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route,
            int cruiseLevel) const -> IntentionCodeData;

        void SetUserControllerPosition(std::string position);
        [[nodiscard]] auto GetUserControllerPosition() const -> std::string;
        [[nodiscard]] static auto GetInvalidExitPointIndex() -> int;

        private:
        [[nodiscard]] auto FoundAllExitPoints(const std::pair<int, int>& indexes) const -> bool;
        void SetExitIndexes(std::pair<int, int>& indexes, int index) const;
        auto FindFirExitPoint(UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& route) const
            -> std::pair<int, int>;

        std::string userControllerPosition;

        // Special airfields
        std::vector<std::unique_ptr<UKControllerPlugin::IntentionCode::AirfieldGroup>> airfieldGroups;

        // Exit Point Repository
        SectorExitRepository& exitPoints;

        // Invalid code - to be used when we have no information for intention codes
        const std::string invalidCode = "--";

        // Used for the exit point index when its not a valid point on the route.
        static const int invalidExitPointIndex = -1;

        // The pair of exit points given if they're not valid
        static constexpr std::pair<int, int> invalidExitPair = {invalidExitPointIndex, invalidExitPointIndex};
    };
} // namespace UKControllerPlugin::IntentionCode
