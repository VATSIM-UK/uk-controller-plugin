#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Holds data regarding the different holds
            available.
        */
        class HoldingDataCollection
        {
            public:
                void Add(UKControllerPlugin::Hold::HoldingData hold);
                int Count(void) const;
                UKControllerPlugin::Hold::HoldingData Get(std::string identifier) const;

                // The hold to return when a hold isnt found
                const UKControllerPlugin::Hold::HoldingData notFound = { "INVALID", 0, 0, 0, 0 };

            private:
                // All the holds
                std::set<UKControllerPlugin::Hold::HoldingData, UKControllerPlugin::Hold::CompareHolds> holds;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
