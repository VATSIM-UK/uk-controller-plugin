#include "PublishedHoldCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Add a published hold
        */
        void PublishedHoldCollection::Add(HoldingData data)
        {
            if (!this->holds.insert(std::move(data)).second) {
                LogWarning("Attempted to add duplicate published hold: " + std::to_string(data.identifier));
            }
        }

        /*
            Get all the published holds at a fix, this is by value so shouldn't be used directly in rendering code.
        */
        const std::set<const HoldingData*> PublishedHoldCollection::GetForFix(std::string fix) const
        {
            std::set<const HoldingData*> holds;

            std::for_each(this->holds.cbegin(), this->holds.cend(), [&fix, &holds](const HoldingData& hold) {
                if (hold.fix != fix) {
                    return;
                }

                holds.insert(&hold);
            });

            return holds;
        }

        /*
         * Get a hold by its id
         */
        const HoldingData& PublishedHoldCollection::GetById(unsigned int id) const
        {
            auto foundHold =
                std::find_if(this->holds.cbegin(), this->holds.cend(), [&id](const HoldingData& hold) -> bool {
                    return hold.identifier == id;
                });

            return foundHold != this->holds.cend() ? *foundHold : this->noHold;
        }

        /*
            Count all the published holds
        */
        size_t PublishedHoldCollection::Count(void) const
        {
            return this->holds.size();
        }

    } // namespace Hold
} // namespace UKControllerPlugin
