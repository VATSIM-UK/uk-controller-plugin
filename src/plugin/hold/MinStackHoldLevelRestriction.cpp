#pragma once
#include "pch/pch.h"

#include "hold/MinStackHoldLevelRestriction.h"
#include "minstack/MinStackLevel.h"

using UKControllerPlugin::MinStack::MinStackLevel;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::SectorFile::Runway;
using UKControllerPlugin::SectorFile::RunwayCollection;

namespace UKControllerPlugin::Hold {

    MinStackHoldLevelRestriction::MinStackHoldLevelRestriction(
        const std::string& airfield,
        unsigned int minStackOffset,
        unsigned int override,
        const MinStackManager& minStackLevels,
        const RunwayCollection& runways,
        std::string runway)
        : minStackLevels(minStackLevels), minStackOffset(minStackOffset), override(override),
          minStackKey(MinStackManager::GetMslKeyAirfield(airfield)), runways(runways), airfield(airfield),
          runway(std::move(runway))
    {
    }

    /*
        Return if a holding level is restricted.
    */
    auto MinStackHoldLevelRestriction::LevelRestricted(unsigned int level) const -> bool
    {
        // Check any runway related restrictions
        if (this->runway != noRunway) {
            const class Runway& selectedRunway =
                this->runways.FetchByIdentifierAndAirfield(this->runway, this->airfield);

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

    auto MinStackHoldLevelRestriction::MinStackKey() const -> const std::string&
    {
        return minStackKey;
    }

    auto MinStackHoldLevelRestriction::MinStackOffset() const -> unsigned int
    {
        return minStackOffset;
    }

    auto MinStackHoldLevelRestriction::Override() const -> unsigned int
    {
        return override;
    }

    auto MinStackHoldLevelRestriction::Runway() const -> const std::string&
    {
        return runway;
    }

    auto MinStackHoldLevelRestriction::Airfield() const -> const std::string&
    {
        return airfield;
    }
} // namespace UKControllerPlugin::Hold
