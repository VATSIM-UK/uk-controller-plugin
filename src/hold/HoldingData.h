#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Stores information about a hold.
        */
        typedef struct HoldingData
        {
            const unsigned int identifier;
            const std::string fix;
            const std::string description;
            const unsigned int minimum;
            const unsigned int maximum;
            const unsigned int inbound;
            const unsigned int turnDirection;

            /*
                Compare two holds
            */
            bool operator== (const HoldingData & compare) const
            {
                return this->identifier == compare.identifier;
            }

            static const unsigned int TURN_DIRECTION_LEFT = 0;
            static const unsigned int TURN_DIRECTION_RIGHT = 1;
        } HoldingData;

    }  // namespace Hold
}  // namespace UKControllerPlugin
