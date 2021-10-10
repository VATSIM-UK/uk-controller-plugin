#pragma once
#include "ownership/ServiceType.h"

namespace UKControllerPlugin::MissedApproach {
    /**
     * Stores global missed approach options such as when to play
     * sound effects.
     */
    class MissedApproachOptions
    {
        public:
        MissedApproachOptions();
        [[nodiscard]] auto AudioAlertForCurrentUser() const -> bool;
        void SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser);
        [[nodiscard]] auto AudioAlertServiceProvisions() const -> Ownership::ServiceType;
        void SetAudioAlertServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions);

        private:
        // Should audio alerts be played if the current user triggered the missed approach
        bool audioAlertForCurrentUser;

        // Should we limit audio alerts to certain service provisions
        Ownership::ServiceType audioAlertServiceProvisions;
    };
} // namespace UKControllerPlugin::MissedApproach
