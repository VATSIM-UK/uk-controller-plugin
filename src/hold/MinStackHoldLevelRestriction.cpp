#pragma once
#include "pch/stdafx.h"
#include "hold/MinStackHoldLevelRestriction.h"
#include "minstack/MinStackLevel.h"

using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackLevel;

namespace UKControllerPlugin {
    namespace Hold {

        MinStackHoldLevelRestriction::MinStackHoldLevelRestriction(
            std::string minStackSubjectAirfield,
            unsigned int minStackOffset,
            const MinStackManager & minStackLevels
        )
            : minStackLevels(minStackLevels), minStackOffset(minStackOffset),
            minStackKey(this->minStackLevels.GetMslKeyAirfield(minStackSubjectAirfield))
        {

        }

        bool MinStackHoldLevelRestriction::LevelRestricted(unsigned int level) const
        {
            const MinStackLevel & msl = this->minStackLevels.GetMinStackLevel(this->minStackKey);

            if (msl == this->minStackLevels.invalidMsl) {
                return false;
            }

            return level < msl.msl + this->minStackOffset;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
