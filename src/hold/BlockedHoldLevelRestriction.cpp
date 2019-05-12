#include "pch/stdafx.h"
#include "hold/BlockedHoldLevelRestriction.h"

namespace UKControllerPlugin {
    namespace Hold {

        BlockedHoldLevelRestriction::BlockedHoldLevelRestriction(std::set<unsigned int> levels)
            : blockedLevels(levels)
        {

        }

        /*
            Get the levels in the restriction
        */
        std::set<unsigned int> BlockedHoldLevelRestriction::GetLevels(void) const
        {
            return this->blockedLevels;
        }

        /*
            The level is restricted if the blocked level matches it
        */
        bool BlockedHoldLevelRestriction::LevelRestricted(unsigned int level) const
        {
            return this->blockedLevels.find(level) != this->blockedLevels.cend();
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
