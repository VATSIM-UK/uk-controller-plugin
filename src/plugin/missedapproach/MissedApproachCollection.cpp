#include "MissedApproach.h"
#include "MissedApproachCollection.h"

namespace UKControllerPlugin::MissedApproach {

    void MissedApproachCollection::Add(const std::shared_ptr<MissedApproach>& missed)
    {
        if (!this->missedApproaches.insert(missed).second) {
            LogWarning("Duplicate missed approach added");
        }
    }

    auto MissedApproachCollection::Count() const -> size_t
    {
        return this->missedApproaches.size();
    }

    auto MissedApproachCollection::Get(const std::string& callsign) const -> std::shared_ptr<MissedApproach>
    {
        auto lock = this->Lock();
        auto missed = this->missedApproaches.find(callsign);
        return missed == this->missedApproaches.cend() ? nullptr : *missed;
    }

    auto MissedApproachCollection::Lock() const -> std::lock_guard<std::mutex>
    {
        return std::lock_guard(this->collectionLock);
    }
} // namespace UKControllerPlugin::MissedApproach
