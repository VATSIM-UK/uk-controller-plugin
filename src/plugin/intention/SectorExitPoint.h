#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeExtractedRouteInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Generic class representing an arbitrary sector exit point. Provides defintions
            for the intention code for a given point, and the direction that constitutes "outward".
        */
        class SectorExitPoint
        {
            public:
                SectorExitPoint(std::string name, std::string intentionCode, unsigned int outDirection);
                virtual ~SectorExitPoint() {}
                virtual std::string GetIntentionCode(
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface & route,
                    int foundPointIndex,
                    int cruiseLevel
                ) const;
                std::string GetName(void) const;
                unsigned int GetOutDirection(void) const;
                bool IsCorrectOutDirection(double directionOfTravel) const;
                virtual bool AppliesToController(std::string callsign) const;

                // The exit directions.
                static const int outNorth = 1;
                static const int outSouth = 2;
                static const int outEast = 4;
                static const int outWest = 8;
                static const int outNorthEast = 16;
                static const int outNorthWest = 32;
                static const int outSouthEast = 64;
                static const int outSouthWest = 128;

            private:
                // Name of the sector exit point
                std::string name;

                // The intention code relating to this sector exit point
                std::string intentionCode;

                // The direction considered to be "out" of the FIR
                unsigned int outDirection;
            };
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
