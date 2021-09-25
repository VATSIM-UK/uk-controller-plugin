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
        auto Get(const std::string& callsign) const -> std::shared_ptr<MissedApproach>;
        [[nodiscard]] auto Count() const -> size_t;

        private:
        [[nodiscard]] auto Lock() const -> std::lock_guard<std::mutex>;

        // Locks the collection for async access
        mutable std::mutex collectionLock;

        // The approaches
        std::set<std::shared_ptr<MissedApproach>, CompareMissedApproaches> missedApproaches;
    };
} // namespace UKControllerPlugin::MissedApproach
