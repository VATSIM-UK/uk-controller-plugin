#pragma once
#include "SectorExitPoint.h"

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            A class that maintains all the Sector Exit pointrs we need.
        */
        class SectorExitRepository
        {
            public:
            SectorExitRepository(
                std::map<std::string, std::unique_ptr<UKControllerPlugin::IntentionCode::SectorExitPoint>> exitMap);
            bool HasSectorExitPoint(std::string point) const;
            const UKControllerPlugin::IntentionCode::SectorExitPoint& GetSectorExitPoint(std::string point) const;

            // The exit directions.
            const int outNorth = 1;
            const int outSouth = 2;
            const int outEast = 4;
            const int outWest = 8;
            const int outNorthEast = 16;
            const int outNorthWest = 32;
            const int outSouthEast = 64;
            const int outSouthWest = 128;

            private:
            std::map<std::string, std::unique_ptr<UKControllerPlugin::IntentionCode::SectorExitPoint>> exitMap;
        };
    } // namespace IntentionCode
} // namespace UKControllerPlugin
