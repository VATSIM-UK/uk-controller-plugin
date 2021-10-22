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
        [[nodiscard]] auto AudioAlert() const -> bool;
        void SetAudioAlert(bool audioAlert);
        [[nodiscard]] auto AudioAlertForCurrentUser() const -> bool;
        void SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser);
        [[nodiscard]] auto ServiceProvisions() const -> Ownership::ServiceType;
        void SetServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions);
        [[nodiscard]] auto Airfields() const -> const std::vector<std::string>&;
        void SetAirfields(std::vector<std::string> airfields);

        private:
        // Should audio alerts be played if the current user triggered the missed approach
        bool audioAlert;

        // Should audio alerts be played if the current user triggered the missed approach
        bool alertForCurrentUser;

        // Should we limit audio alerts to certain service provisions
        Ownership::ServiceType seviceProvisions;

        // Which airfields we care about
        std::vector<std::string> airfields;
    };
} // namespace UKControllerPlugin::MissedApproach
