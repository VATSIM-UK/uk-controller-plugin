#pragma once

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Stores information about a hold.
        */
        typedef struct HoldingData
        {
            unsigned int identifier;
            std::string fix;
            std::string description;
            unsigned int minimum;
            unsigned int maximum;
            unsigned int inbound;
            std::string turnDirection;

            /*
                Compare two holds
            */
            bool operator== (const HoldingData & compare) const
            {
                return this->identifier == compare.identifier;
            }

            static const std::string TURN_DIRECTION_LEFT;
            static const std::string TURN_DIRECTION_RIGHT;
        } HoldingData;
    }  // namespace Hold
}  // namespace UKControllerPlugin
