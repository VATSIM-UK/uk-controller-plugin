#include "MissedApproach.h"
#include "MissedApproachCollection.h"

namespace UKControllerPlugin::MissedApproach {

    void MissedApproachCollection::Add(const std::shared_ptr<MissedApproach>& missed)
    {
        auto lock = this->Lock();
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
        return this->FirstWhere([&callsign](const std::shared_ptr<MissedApproach>& missed) -> bool {
            return missed->Callsign() == callsign;
        });
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

    auto MissedApproachCollection::FirstWhere(
        const std::function<bool(const std::shared_ptr<MissedApproach>&)>& predicate) const
        -> std::shared_ptr<MissedApproach>
    {
        auto lock = this->Lock();
        for (const auto& approach : this->missedApproaches) {
            if (predicate(approach)) {
                return approach;
            }
        }

        return nullptr;
    }

    void MissedApproachCollection::Remove(const std::shared_ptr<MissedApproach>& missed)
    {
        auto lock = this->Lock();
        this->missedApproaches.erase(missed);
    }

    auto MissedApproachCollection::Get(int id) const -> std::shared_ptr<MissedApproach>
    {
        return this->FirstWhere(
            [&id](const std::shared_ptr<MissedApproach>& missed) -> bool { return missed->Id() == id; });
    }
} // namespace UKControllerPlugin::MissedApproach
