#include "pch/stdafx.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {

        ManagedHold::ManagedHold(HoldingData holdParameters)
            : holdParameters(std::move(holdParameters)), uniqueHoldLock(this->holdLock, std::defer_lock_t())
        {

        }

        ManagedHold::ManagedHold(ManagedHold && compare)
            : holdParameters(std::move(compare.holdParameters)), uniqueHoldLock(this->holdLock, std::defer_lock_t())
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

        /*
            Don't destroy until we know that the mutex is free
        */
        ManagedHold::~ManagedHold()
        {
            std::lock_guard<std::mutex> aircraftListLock(this->holdLock);
        }

        /*
            Remove any current holding aircraft information from the hold and add the updated information.
        */
        void ManagedHold::AddHoldingAircraft(HoldingAircraft aircraft)
        {
            std::lock_guard<std::mutex> aircraftListLock(this->holdLock);
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
            Lock the mutex on the aircraft list so that iteration may be performed
        */
        void ManagedHold::LockAircraftList(void) const
        {
            this->uniqueHoldLock.lock();
        }

        /*
            Remove an aircraft from the hold.
        */
        void ManagedHold::RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft)
        {
            std::lock_guard<std::mutex> aircraftListLock(this->holdLock);
            this->holdingAircraft.remove(aircraft);
        }

        /*
            Remove an aircraft from the hold by callsign
        */
        void ManagedHold::RemoveHoldingAircraft(std::string callsign)
        {
            std::lock_guard<std::mutex> aircraftListLock(this->holdLock);
            auto aircraft = std::find(this->holdingAircraft.cbegin(), this->holdingAircraft.cend(), callsign);
            if (aircraft == this->holdingAircraft.end()) {
                return;
            }

            this->holdingAircraft.erase(aircraft);
        }

        /*
            Unlock the aircraft list
        */
        void ManagedHold::UnlockAircraftList(void) const
        {
            if (!this->uniqueHoldLock.owns_lock()) {
                return;
            }
            this->uniqueHoldLock.unlock();
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
