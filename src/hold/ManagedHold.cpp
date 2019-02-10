#include "pch/stdafx.h"
#include "hold/ManagedHold.h"

using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {

        ManagedHold::ManagedHold(HoldingData holdParameters)
            : holdParameters(holdParameters), uniqueHoldLock(this->holdLock, std::defer_lock_t())
        {

        }

        ManagedHold::ManagedHold(const ManagedHold & compare)
            : holdParameters(compare.holdParameters), uniqueHoldLock(this->holdLock, std::defer_lock_t())
        {
            this->holdingAircraft = compare.holdingAircraft;
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
            this->holdingAircraft.insert(aircraft);
        }

        /*
            Number of aircraft in the hold. Locking not required here as access is safe.
        */
        size_t ManagedHold::CountHoldingAircraft(void) const
        {
            return this->holdingAircraft.size();
        }

        /*
            Is the aircraft in the hold. Locking not required here as access is safe.
        */
        bool ManagedHold::HasAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft) const
        {
            return this->holdingAircraft.count(aircraft) != 0;
        }

        /*
            Is the aircraft in the hold. Locking not required here as access is safe.
        */
        bool ManagedHold::HasAircraft(std::string callsign) const
        {
            return this->holdingAircraft.count(callsign) != 0;
        }

        /*
            Lock the mutex on the aircraft list so that iteration may be performed
        */
        void ManagedHold::LockAircraftList(void)
        {
            this->uniqueHoldLock.lock();
        }

        /*
            Remove an aircraft from the hold.
        */
        void ManagedHold::RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft)
        {
            std::lock_guard<std::mutex> aircraftListLock(this->holdLock);
            this->holdingAircraft.erase(aircraft);
        }

        /*
            Remove an aircraft from the hold by callsign
        */
        void ManagedHold::RemoveHoldingAircraft(std::string callsign)
        {
            std::lock_guard<std::mutex> aircraftListLock(this->holdLock);
            auto aircraft = this->holdingAircraft.find(callsign);
            if (aircraft == this->holdingAircraft.end()) {
                return;
            }

            this->holdingAircraft.erase(aircraft);
        }

        /*
            Unlock the aircraft list
        */
        void ManagedHold::UnlockAircraftList(void)
        {
            if (!this->uniqueHoldLock.owns_lock()) {
                return;
            }
            this->uniqueHoldLock.unlock();
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
