#pragma once
#include "hold/AbstractHoldLevelRestriction.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Represents a restriction where a set of levels in the hold are permanently unusable
            or not likely to change on a regular basis.
        */
        class BlockedHoldLevelRestriction : public UKControllerPlugin::Hold::AbstractHoldLevelRestriction
        {
            public:
            explicit BlockedHoldLevelRestriction(std::set<unsigned int> levels);

            // Inherited via AbstractHoldLevelRestriction
            std::set<unsigned int> GetLevels(void) const;
            bool LevelRestricted(unsigned int level) const override;

            private:
            // The level that is blocked
            const std::set<unsigned int> blockedLevels;
        };

    } // namespace Hold
} // namespace UKControllerPlugin
