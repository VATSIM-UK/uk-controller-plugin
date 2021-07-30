#pragma once
#include "hold/AbstractHoldLevelRestriction.h"
#include "minstack/MinStackManager.h"
#include "sectorfile/RunwayCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that represents a holding restriction
            based on the Minimum Stack Level
        */
        class MinStackHoldLevelRestriction : public UKControllerPlugin::Hold::AbstractHoldLevelRestriction
        {

            public:
                MinStackHoldLevelRestriction(
                    std::string airfield,
                    unsigned int minStackOffset,
                    unsigned int override,
                    const UKControllerPlugin::MinStack::MinStackManager & minStackLevels,
                    const UKControllerPlugin::SectorFile::RunwayCollection & runways,
                    std::string runway = ""
                );

                // Inherited via AbstractHoldLevelRestriction
                bool LevelRestricted(unsigned int level) const override;

                // The subject airfield for minimum stack levels
                const std::string minStackKey;

                // The overriding level for any restriction
                const unsigned int override;

                // The offset against minstack levels - e.g. MSL+1 would be 1000
                const unsigned int minStackOffset;

                // The runways that this restriction applies to
                const std::string runway;

                const std::string airfield;

            private:

                // Manages the Minimum Stack Levels
                const UKControllerPlugin::MinStack::MinStackManager & minStackLevels;

                // Looks after the active runways
                const UKControllerPlugin::SectorFile::RunwayCollection & runways;

                // No runway is selected for this restriction
                const std::string noRunway = "";
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
