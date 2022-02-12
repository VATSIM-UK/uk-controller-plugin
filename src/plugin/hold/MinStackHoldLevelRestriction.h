#pragma once
#include "AbstractHoldLevelRestriction.h"

namespace UKControllerPlugin {
    namespace MinStack {
        class MinStackManager;
    } // namespace MinStack
    namespace SectorFile {
        class RunwayCollection;
    } // namespace SectorFile
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Hold {

    /*
        A class that represents a holding restriction
        based on the Minimum Stack Level
    */
    class MinStackHoldLevelRestriction : public UKControllerPlugin::Hold::AbstractHoldLevelRestriction
    {

        public:
        MinStackHoldLevelRestriction(
            const std::string& airfield,
            unsigned int minStackOffset,
            unsigned int override,
            const UKControllerPlugin::MinStack::MinStackManager& minStackLevels,
            const UKControllerPlugin::SectorFile::RunwayCollection& runways,
            std::string runway = "");

        [[nodiscard]] auto LevelRestricted(unsigned int level) const -> bool override;
        [[nodiscard]] auto MinStackKey() const -> const std::string&;
        [[nodiscard]] auto MinStackOffset() const -> unsigned int;
        [[nodiscard]] auto Override() const -> unsigned int;
        [[nodiscard]] auto Runway() const -> const std::string&;
        [[nodiscard]] auto Airfield() const -> const std::string&;

        private:
        // Manages the Minimum Stack Levels
        const UKControllerPlugin::MinStack::MinStackManager& minStackLevels;

        // Looks after the active runways
        const UKControllerPlugin::SectorFile::RunwayCollection& runways;

        // No runway is selected for this restriction
        const std::string noRunway;

        // The subject airfield for minimum stack levels
        const std::string minStackKey;

        // The overriding level for any restriction
        const unsigned int override;

        // The offset against minstack levels - e.g. MSL+1 would be 1000
        const unsigned int minStackOffset;

        // The airfield that this restriction applies to
        const std::string airfield;

        // The runways that this restriction applies to
        const std::string runway;
    };
} // namespace UKControllerPlugin::Hold
