#include "pch/stdafx.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {

        ManagedHold::ManagedHold(HoldingData holdParameters)
            : holdParameters(holdParameters)
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
            Number of aircraft in the hold
        */
        size_t ManagedHold::CountHoldingAircraft(void) const
        {
            return this->holdingAircraft.size();
        }

        /*
            Is the aircraft in the hold
        */
        bool ManagedHold::HasAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft) const
        {
            return this->holdingAircraft.count(aircraft) != 0;
        }

        /*
            Is the aircraft in the hold
        */
        bool ManagedHold::HasAircraft(std::string callsign) const
        {
            return this->holdingAircraft.count(callsign) != 0;
        }

        /*
            Remove an aircraft from the hold
        */
        void ManagedHold::RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft)
        {
            this->holdingAircraft.erase(aircraft);
        }

        /*
            Remove an aircraft from the hold by callsign
        */
        void ManagedHold::RemoveHoldingAircraft(std::string callsign)
        {
            auto aircraft = this->holdingAircraft.find(callsign);
            if (aircraft == this->holdingAircraft.end()) {
                return;
            }

            this->holdingAircraft.erase(aircraft);
        }

        /*
            Compare two holds
        */
        bool ManagedHold::operator== (const ManagedHold & compare) const
        {
            return this->holdParameters == compare.holdParameters;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
