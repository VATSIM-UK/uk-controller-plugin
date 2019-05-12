#include "pch/stdafx.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {

        ManagedHold::ManagedHold(HoldingData holdParameters)
            : holdParameters(std::move(holdParameters))
        {

        }

        ManagedHold::ManagedHold(ManagedHold && compare)
            : holdParameters(std::move(compare.holdParameters))
        {
            this->holdingAircraft = compare.holdingAircraft;
        }

        ManagedHold & ManagedHold::operator=(ManagedHold && original)
        {
            if (this != &original)
            {
                this->holdParameters = std::move(original.holdParameters);
            }

            return *this;
        }

        ManagedHold::~ManagedHold()
        {

        }


        /*
            Remove any current holding aircraft information from the hold and add the updated information.
        */
        void ManagedHold::AddHoldingAircraft(HoldingAircraft aircraft)
        {
            if (this->HasAircraft(aircraft)) {
                return;
            }
            this->holdingAircraft.push_back(aircraft);
        }

        /*
            Number of aircraft in the hold. Locking not required here as access is safe.
        */
        size_t ManagedHold::CountHoldingAircraft(void) const
        {
            return this->holdingAircraft.size();
        }

        /*
            Get the parameters
        */
        const UKControllerPlugin::Hold::HoldingData & ManagedHold::GetHoldParameters(void) const
        {
            return this->holdParameters;
        }

        /*
            Return the number of levels in the hold for use
        */
        unsigned int ManagedHold::GetNumberOfLevels(void) const
        {
            return ((this->holdParameters.maximum - this->holdParameters.minimum) / 1000) + 1;
        }

        /*
            Is the aircraft in the hold. Locking not required here as access is safe.
        */
        bool ManagedHold::HasAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft) const
        {
            return std::find(this->holdingAircraft.cbegin(), this->holdingAircraft.cend(), aircraft) !=
                this->holdingAircraft.cend();
        }

        /*
            Is the aircraft in the hold. Locking not required here as access is safe.
        */
        bool ManagedHold::HasAircraft(std::string callsign) const
        {
            return std::find(this->holdingAircraft.cbegin(), this->holdingAircraft.cend(), callsign) !=
                this->holdingAircraft.cend();
        }

        /*
            Remove an aircraft from the hold.
        */
        void ManagedHold::RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft)
        {
            this->holdingAircraft.remove(aircraft);
        }

        /*
            Remove an aircraft from the hold by callsign
        */
        void ManagedHold::RemoveHoldingAircraft(std::string callsign)
        {
            auto aircraft = std::find(this->holdingAircraft.cbegin(), this->holdingAircraft.cend(), callsign);
            if (aircraft == this->holdingAircraft.end()) {
                return;
            }

            this->holdingAircraft.erase(aircraft);
        }

        /*
            Updates an aircraft in the hold
        */
        void ManagedHold::UpdateHoldingAircraft(
            std::string callsign,
            int clearedLevel,
            int reportedLevel,
            int verticalSpeed
        ) {
            if (!this->HasAircraft(callsign)) {
                return;
            }

            auto aircraft = std::find(this->holdingAircraft.begin(), this->holdingAircraft.end(), callsign);
            aircraft->clearedLevel = clearedLevel;
            aircraft->reportedLevel = reportedLevel;
            aircraft->verticalSpeed = verticalSpeed;
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
