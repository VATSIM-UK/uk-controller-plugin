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
                const std::set<const HoldingData*> GetForFix(std::string fix) const;
                const HoldingData& GetById(int id) const;
                size_t Count(void) const;

                // Returns the hold with no data
                const HoldingData noHold = {};

            private:

                // Array to return if no holds are found
                const std::set<HoldingData, CompareHolds> noHolds;

                // All the published holds
                std::set<HoldingData, CompareHolds> holds;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
