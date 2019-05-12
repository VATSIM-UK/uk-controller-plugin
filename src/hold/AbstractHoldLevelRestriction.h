#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Represents a restriction on the levels in a hold that may be utilised.
        */
        class AbstractHoldLevelRestriction
        {
            public:

                /*
                    Given a level, return true if it is off limits, false if not.
                */
                virtual bool LevelRestricted(unsigned int level) const = 0;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
