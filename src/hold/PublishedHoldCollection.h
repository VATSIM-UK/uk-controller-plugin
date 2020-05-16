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
                const std::set<HoldingData, CompareHolds>& Get(std::string fix);
                size_t Count(void) const;

            private:

                // All the published holds
                std::map<std::string, std::set<HoldingData, CompareHolds>> holds;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
