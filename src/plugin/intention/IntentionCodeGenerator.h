#pragma once
#include "intention/SectorExitRepository.h"

// Need to make the vector of groups a pointer to resolve this.
#include "intention/AirfieldGroup.h"
#include "intention/IntentionCodeData.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
    namespace IntentionCode {
        class SectorExitRepository;
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        // Pre Declarations
        class AirfieldGroup;
        // END

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
                    UKControllerPlugin::IntentionCode::SectorExitRepository & exitPoints
                );
                UKControllerPlugin::IntentionCode::IntentionCodeData GetIntentionCodeForFlightplan(
                    std::string callsign,
                    std::string origin,
                    std::string destination,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route,
                    int cruiseLevel
                );

                void SetUserControllerPosition(std::string position);
                std::string GetUserControllerPosition(void) const;

                // Invalid code - to be used when we have no information for intention codes
                const std::string invalidCode = "--";

                // Used for the exit point index when its not a valid point on the route.
                static const int invalidExitPointIndex = -1;

                // The value given to us by Euroscope if we try to find the time to a point that has been passed.
                const int exitPointPassed = -1;

            private:

                std::string userControllerPosition = "";

                int FindFirExitPoint(UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route);

                // Special airfields
                std::vector<std::unique_ptr<UKControllerPlugin::IntentionCode::AirfieldGroup>> airfieldGroups;

                // Exit Point Repository
                SectorExitRepository & exitPoints;
            };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
