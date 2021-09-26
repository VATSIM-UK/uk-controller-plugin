#pragma once
#include "CompareMissedApproaches.h"

namespace UKControllerPlugin::MissedApproach {
    class MissedApproach;

    /**
     * A collection of all the missed approaches being tracked
     * by the plugin.
     */
    class MissedApproachCollection
    {
        public:
        void Add(const std::shared_ptr<MissedApproach>& missed);
        [[nodiscard]] auto
        FirstWhere(const std::function<bool(const std::shared_ptr<MissedApproach>&)>& predicate) const
            -> std::shared_ptr<MissedApproach>;
        [[nodiscard]] auto Get(const std::string& callsign) const -> std::shared_ptr<MissedApproach>;
        [[nodiscard]] auto Count() const -> size_t;
        void Remove(const std::shared_ptr<MissedApproach>& missed);
        void RemoveWhere(const std::function<bool(const std::shared_ptr<MissedApproach>&)>& predicate);

        private:
        [[nodiscard]] auto Lock() const -> std::lock_guard<std::mutex>;

        // Locks the collection for async access
        mutable std::mutex collectionLock;

        // The approaches
        std::set<std::shared_ptr<MissedApproach>, CompareMissedApproaches> missedApproaches;
    };
} // namespace UKControllerPlugin::MissedApproach
