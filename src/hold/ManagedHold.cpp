#include "pch/stdafx.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::HoldingAircraft;

namespace UKControllerPlugin {
    namespace Hold {

        ManagedHold::ManagedHold(
            const unsigned int identifier,
            const std::string fix,
            const std::string description,
            const unsigned int minimum,
            const unsigned int maximum,
            const unsigned int inbound,
            const unsigned int turnDirection
        )
            : identifier(identifier), fix(fix), description(description), minimum(minimum), maximum(maximum),
            inbound(inbound), turnDirection(turnDirection)
        {

        }

        /*
            Remove any current holding aircraft information from the hold and add the updated information.
        */
        void ManagedHold::AddHoldingAircraft(HoldingAircraft aircraft)
        {
            this->holdingAircraft.insert(aircraft);
        }

        /*
            Remove an aircraft from the hold
        */
        void ManagedHold::RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft)
        {
            this->holdingAircraft.erase(aircraft);
        }

        /*
            Compare two holds
        */
        bool ManagedHold::operator== (const ManagedHold & compare) const
        {
            return this->identifier == compare.identifier;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
