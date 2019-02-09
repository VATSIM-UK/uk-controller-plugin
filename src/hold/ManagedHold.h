#pragma once
#include "hold/HoldingAircraft.h"
#include "hold/CompareHoldingAircraft.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Stores information about a hold and the aircraft
            in it.
        */
        class ManagedHold
        {
            public:
                ManagedHold(
                    const unsigned int identifier,
                    const std::string fix,
                    const std::string description,
                    const unsigned int minimum,
                    const unsigned int maximum,
                    const unsigned int inbound,
                    const unsigned int turnDirection
                );
                void AddHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft);
                void RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft);
                bool operator== (const ManagedHold & compare) const;


                // Public type definitions for a custom iterator over the class.
                typedef std::set<
                    UKControllerPlugin::Hold::HoldingAircraft,
                    UKControllerPlugin::Hold::CompareHoldingAircraft
                > ManagedHoldAircraft;
                typedef ManagedHoldAircraft::const_iterator const_iterator;
                const_iterator cbegin(void) const { return this->holdingAircraft.cbegin(); }
                const_iterator cend(void) const { return holdingAircraft.cend(); }


                // The ID of the hold from the web API
                const unsigned int identifier;

                // The fixed point that the hold centers around
                const std::string fix;

                // The description of the hold (usually the fix name, but not always)
                const std::string description;

                // The minimum possible altitude in the hold
                const unsigned int minimum;

                // The maximum possible altitude in the hold
                const unsigned int maximum;

                // The inbound heading for the hold
                const unsigned int inbound;

                // The turn direction at the fix
                const unsigned int turnDirection;

                static const unsigned int TURN_DIRECTION_LEFT = 0;
                static const unsigned int TURN_DIRECTION_RIGHT = 1;

            private:

                // The aircraft in the hold
                std::set<
                    UKControllerPlugin::Hold::HoldingAircraft,
                    UKControllerPlugin::Hold::CompareHoldingAircraft
                > holdingAircraft;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin

