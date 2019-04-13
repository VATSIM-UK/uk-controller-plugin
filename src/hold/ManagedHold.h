#pragma once
#include "hold/HoldingAircraft.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/HoldingData.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Stores information about a hold and the aircraft
            in it.
        */
        class ManagedHold
        {
            public:
                ManagedHold(const UKControllerPlugin::Hold::HoldingData holdData);
                ManagedHold(ManagedHold const &) = delete;
                ManagedHold &operator=(ManagedHold const &) = delete;
                ManagedHold(ManagedHold && original);
                ManagedHold &operator=(ManagedHold && original);
                ~ManagedHold();
                void AddHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft);
                size_t CountHoldingAircraft(void) const;
                const UKControllerPlugin::Hold::HoldingData & GetHoldParameters(void) const;
                unsigned int GetNumberOfLevels(void) const;
                bool HasAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft) const;
                bool HasAircraft(std::string callsign) const;
                void LockAircraftList(void) const;
                void RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft);
                void RemoveHoldingAircraft(std::string callsign);
                void UnlockAircraftList(void) const;
                void UpdateHoldingAircraft(
                    std::string callsign,
                    int clearedLevel,
                    int reportedLevel,
                    int verticalSpeed
                );
                bool operator== (const ManagedHold & compare) const;


                // Public type definitions for a custom iterator over the class.
                typedef std::list<UKControllerPlugin::Hold::HoldingAircraft> ManagedHoldAircraft;
                typedef ManagedHoldAircraft::const_iterator const_iterator;
                const_iterator cbegin(void) const { return this->holdingAircraft.cbegin(); }
                const_iterator cend(void) const { return holdingAircraft.cend(); }

            private:

                // The parameters of the hold
                UKControllerPlugin::Hold::HoldingData holdParameters;

                // The aircraft in the hold
                std::list<UKControllerPlugin::Hold::HoldingAircraft> holdingAircraft;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin

