#pragma once
#include "euroscope/UserSettingAwareInterface.h"
#include "ownership/ServiceType.h"

namespace UKControllerPlugin::Euroscope {
    class UserSetting;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::MissedApproach {
    /**
     * Stores global missed approach options such as when to play
     * sound effects.
     */
    class MissedApproachOptions : public Euroscope::UserSettingAwareInterface
    {
        public:
        MissedApproachOptions(Euroscope::UserSetting& userSettings);
        void UserSettingsUpdated(Euroscope::UserSetting& userSettings) override;
        [[nodiscard]] auto AudioAlertForCurrentUser() const -> bool;
        void SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser);
        [[nodiscard]] auto AudioAlertServiceProvisions() const -> Ownership::ServiceType;
        void SetAudioAlertServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions);

        private:
        // Where we persist user settings
        Euroscope::UserSetting& userSettings;

        // Should audio alerts be played if the current user triggered the missed approach
        bool audioAlertForCurrentUser;

        // Should we limit audio alerts to certain service provisions
        Ownership::ServiceType audioAlertServiceProvisions;

        const std::string CURRENT_USER_AUDIO_ASR_KEY = "missedApproachAudioCurrentUser";
        const std::string CURRENT_USER_AUDIO_ASR_DESC = "Play Missed Approach Alarm If User Initiated";
        const std::string SERVICE_PROVISION_ASR_KEY = "missedApproachAudioServiceProvision";
        const std::string SERVICE_PROVISION_ASR_DESC = "Missed Approach Audio For Service Provision";
    };
} // namespace UKControllerPlugin::MissedApproach
