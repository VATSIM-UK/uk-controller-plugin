#include "pch/stdafx.h"
#include "hold/PublishedHoldCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Add a published hold
        */
        void PublishedHoldCollection::Add(HoldingData data)
        {
            if (!this->holds[data.fix].insert(std::move(data)).second) {
                LogWarning("Attempted to add duplicate published hold: " + std::to_string(data.identifier));
            }
        }

        /*
            Get all the published holds at a fix
        */
        const std::set<HoldingData, CompareHolds>& PublishedHoldCollection::Get(std::string fix) const
        {
            return
                this->holds.count(fix)
                ? this->holds.at(fix)
                : this->noHolds;
        }

        /*
            Count all the published holds
        */
        size_t PublishedHoldCollection::Count(void) const
        {
            size_t count = 0;
            for (
                std::map<std::string, std::set<HoldingData, CompareHolds>>::const_iterator it = this->holds.cbegin();
                it != this->holds.cend();
                ++it
            ) {
                count += it->second.size();
            }

            return count;
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
