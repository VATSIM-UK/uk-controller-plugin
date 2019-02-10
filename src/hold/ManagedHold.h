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
                void AddHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft);
                size_t CountHoldingAircraft(void) const;
                bool HasAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft) const;
                bool HasAircraft(std::string callsign) const;
                void RemoveHoldingAircraft(UKControllerPlugin::Hold::HoldingAircraft aircraft);
                void RemoveHoldingAircraft(std::string callsign);
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
                const UKControllerPlugin::Hold::HoldingData holdParameters;

            private:

                // The aircraft in the hold
                std::set<
                    UKControllerPlugin::Hold::HoldingAircraft,
                    UKControllerPlugin::Hold::CompareHoldingAircraft
                > holdingAircraft;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin

