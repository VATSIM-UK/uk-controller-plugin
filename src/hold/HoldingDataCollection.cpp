#include "pch/stdafx.h"
#include "hold/HoldingDataCollection.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Add a hold to the collection
        */
        void HoldingDataCollection::Add(UKControllerPlugin::Hold::HoldingData hold)
        {
            if (!this->holds.insert(hold).second) {
                LogWarning("Duplicate hold added: " + hold.identifier);
            }
        }

        /*
            Count then number of holds
        */
        int HoldingDataCollection::Count(void) const
        {
            return this->holds.size();
        }

        /*
            Get a hold
        */
        UKControllerPlugin::Hold::HoldingData HoldingDataCollection::Get(unsigned int identifier) const
        {
            auto hold = this->holds.find(identifier);
            return hold != this->holds.end() ? *hold : this->notFound;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
