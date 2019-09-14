#pragma once
#include "hold/AbstractHoldLevelRestriction.h"
#include "minstack/MinStackManager.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that represents a holding restriction
            based on the Minimum Stack Level
        */
        class MinStackHoldLevelRestriction : UKControllerPlugin::Hold::AbstractHoldLevelRestriction
        {

            public: 
                MinStackHoldLevelRestriction(
                    std::string minStackSubjectAirfield,
                    unsigned int minStackOffset,
                    const UKControllerPlugin::MinStack::MinStackManager & minStackLevels
                );

                // Inherited via AbstractHoldLevelRestriction
                virtual bool LevelRestricted(unsigned int level) const override;

            private:

                // Manages the Minimum Stack Levels
                const UKControllerPlugin::MinStack::MinStackManager & minStackLevels;

                // The subject airfield for minimum stack levels
                const std::string minStackKey;

                // The offset against minstack levels - e.g. MSL+1 would be 1000
                const unsigned int minStackOffset;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
