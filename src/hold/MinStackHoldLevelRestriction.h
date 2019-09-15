#pragma once
#include "hold/AbstractHoldLevelRestriction.h"
#include "minstack/MinStackManager.h"

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
                    std::string minStackSubjectAirfield,
                    unsigned int minStackOffset,
                    unsigned int override,
                    const UKControllerPlugin::MinStack::MinStackManager & minStackLevels
                );

                // Inherited via AbstractHoldLevelRestriction
                virtual bool LevelRestricted(unsigned int level) const override;

                // The subject airfield for minimum stack levels
                const std::string minStackKey;

                // The overriding level for any restriction
                const unsigned int override;

                // The offset against minstack levels - e.g. MSL+1 would be 1000
                const unsigned int minStackOffset;

            private:

                // Manages the Minimum Stack Levels
                const UKControllerPlugin::MinStack::MinStackManager & minStackLevels;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
