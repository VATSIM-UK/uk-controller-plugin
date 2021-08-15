#pragma once
#include "hold/MinStackHoldLevelRestriction.h"
#include "minstack/MinStackLevel.h"
#include "pch/pch.h"

using UKControllerPlugin::MinStack::MinStackLevel;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::SectorFile::Runway;
using UKControllerPlugin::SectorFile::RunwayCollection;

namespace UKControllerPlugin {
    namespace Hold {

        MinStackHoldLevelRestriction::MinStackHoldLevelRestriction(
            std::string airfield,
            unsigned int minStackOffset,
            unsigned int override,
            const MinStackManager& minStackLevels,
            const RunwayCollection& runways,
            std::string runway)
            : minStackLevels(minStackLevels), minStackOffset(minStackOffset), override(override),
              minStackKey(this->minStackLevels.GetMslKeyAirfield(airfield)), runways(runways), airfield(airfield),
              runway(runway)
        {
        }

        /*
            Return if a holding level is restricted.
        */
        bool MinStackHoldLevelRestriction::LevelRestricted(unsigned int level) const
        {
            // Check any runway related restrictions
            if (this->runway != noRunway) {
                const Runway& selectedRunway = this->runways.FetchByIdentifierAndAirfield(this->runway, this->airfield);

                if (selectedRunway == this->runways.invalidRunway || !selectedRunway.Active()) {
                    return false;
                }
            }

            // Check overrides
            if (level < this->override) {
                return true;
            }

            // Check against the MSL
            const MinStackLevel& msl = this->minStackLevels.GetMinStackLevel(this->minStackKey);

            if (msl == this->minStackLevels.InvalidMsl()) {
                return false;
            }

            return level < msl.msl + this->minStackOffset;
        }
    } // namespace Hold
} // namespace UKControllerPlugin
