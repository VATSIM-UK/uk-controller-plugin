#include "pch/pch.h"
#include "handoff/HandoffCollection.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin {
    namespace Handoff {

        void HandoffCollection::AddHandoffOrder(
            std::string key,
            std::shared_ptr<ControllerPositionHierarchy> controllers
        ) {
            if (this->orders.count(key)) {
                LogWarning("Duplicate handoff order detected for " + key);
                return;
            }

            this->orders[key] = std::move(controllers);
        }

        void HandoffCollection::AddSidMapping(std::string airfield, std::string identifier, std::string handoffKey)
        {
            if (this->sidMappings.count(this->GetStorageKeyForSid(airfield, identifier))) {
                LogWarning("Duplicate sid mapping detected for " + this->GetStorageKeyForSid(airfield, identifier));
                return;
            }

            this->sidMappings[this->GetStorageKeyForSid(airfield, identifier)] = handoffKey;
        }

        const ControllerPositionHierarchy& HandoffCollection::GetSidHandoffOrder(
            std::string airfield,
            std::string identifier
        ) const {
            std::string storageKey = this->GetStorageKeyForSid(airfield, identifier);
            if (!this->sidMappings.count(storageKey)) {
                LogWarning("No SID mapping available for " + storageKey);
                return this->invalidHierarchy;
            }

            std::string handoffKey = this->sidMappings.at(storageKey);
            if (!this->orders.count(handoffKey)) {
                LogWarning("Handoff order " + handoffKey + " not found for SID " + storageKey);
                return this->invalidHierarchy;
            }

            return *this->orders.at(handoffKey);
        }

        size_t HandoffCollection::CountSidMappings(void) const
        {
            return this->sidMappings.size();
        }

        size_t HandoffCollection::CountHandoffs(void) const
        {
            return this->orders.size();
        }

        std::string HandoffCollection::GetStorageKeyForSid(std::string airfield, std::string identifier) const
        {
            return airfield + "." + identifier;
        }

    }  // namespace Handoff
}  // namespace UKControllerPlugin
