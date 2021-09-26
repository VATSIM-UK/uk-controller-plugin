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

    void
    MissedApproachCollection::RemoveWhere(const std::function<bool(const std::shared_ptr<MissedApproach>&)>& predicate)
    {
        auto lock = this->Lock();
        for (auto missedApproach = this->missedApproaches.cbegin(); missedApproach != this->missedApproaches.cend();) {
            if (predicate(*missedApproach)) {
                missedApproach = this->missedApproaches.erase(missedApproach);
            } else {
                ++missedApproach;
            }
        }
    }
} // namespace UKControllerPlugin::MissedApproach
