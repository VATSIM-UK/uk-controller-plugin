#pragma once
#include "hold/HoldingData.h"
#include "hold/CompareHolds.h"

namespace UKControllerPlugin {
    namespace Hold {
        /*
            A new class
        */
        class PublishedHoldCollection
        {
            public:
                void Add(HoldingData data);
                const std::set<HoldingData, CompareHolds>& Get(std::string fix) const;
                size_t Count(void) const;

            private:

                // Array to return if no holds are found
                const std::set<HoldingData, CompareHolds> noHolds;

                // All the published holds
                std::map<std::string, std::set<HoldingData, CompareHolds>> holds;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
