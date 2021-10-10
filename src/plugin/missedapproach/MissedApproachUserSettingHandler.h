#pragma once
#include "euroscope/UserSettingAwareInterface.h"

namespace UKControllerPlugin::Euroscope {
    class UserSetting;
    class UserSettingAwareCollection;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachOptions;

    /**
     * A class that handles the saving the global (non-ASR) settings for missed
     * approaches to Euroscope settings. This is separate to reduce the dependencies on
     * the main options class.
     */
    class MissedApproachUserSettingHandler : public Euroscope::UserSettingAwareInterface
    {
        public:
        MissedApproachUserSettingHandler(
            std::shared_ptr<MissedApproachOptions> options,
            const Euroscope::UserSettingAwareCollection& userSettingsHandlers,
            Euroscope::UserSetting& userSettings);
        void UserSettingsUpdated(Euroscope::UserSetting& userSettings) override;
        void SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser);
        void SetAudioAlertServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions);

        private:
        
        // The options
        std::shared_ptr<MissedApproachOptions> options;
        
        // Who to let know when something changes
        const Euroscope::UserSettingAwareCollection& userSettingsHandlers;

        // Where we persist user settings
        Euroscope::UserSetting& userSettings;

        const std::string CURRENT_USER_AUDIO_ASR_KEY = "missedApproachAudioCurrentUser";
        const std::string CURRENT_USER_AUDIO_ASR_DESC = "Play Missed Approach Alarm If User Initiated";
        const std::string SERVICE_PROVISION_ASR_KEY = "missedApproachAudioServiceProvision";
        const std::string SERVICE_PROVISION_ASR_DESC = "Missed Approach Audio For Service Provision";
    };
} // namespace UKControllerPlugin::MissedApproach
