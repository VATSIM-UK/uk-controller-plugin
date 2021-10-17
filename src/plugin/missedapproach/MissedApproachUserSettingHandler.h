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
        void SetAudioAlert(bool audioAlert);
        void SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser);
        void SetServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions);
        void SetAirfields(std::vector<std::string> airfields);
        [[nodiscard]] auto GetOptions() const -> const std::shared_ptr<const MissedApproachOptions>;

        private:
        // The options
        std::shared_ptr<MissedApproachOptions> options;

        // Who to let know when something changes
        const Euroscope::UserSettingAwareCollection& userSettingsHandlers;

        // Where we persist user settings
        Euroscope::UserSetting& userSettings;

        const std::string AUDIO_KEY = "missedApproachAudio";
        const std::string AUDIO_DESC = "Play Missed Approach Alarm";
        const std::string CURRENT_USER_AUDIO_KEY = "missedApproachAudioCurrentUser";
        const std::string CURRENT_USER_AUDIO_DESC = "Play Missed Approach Alarm If User Initiated";
        const std::string SERVICE_PROVISION_KEY = "missedApproachServiceProvision";
        const std::string SERVICE_PROVISION_DESC = "Missed Approach Alerts Service Provision";
        const std::string AIRFIELDS_KEY = "missedApproachAirfields";
        const std::string AIRFIELDS_DESC = "Missed Approach Alert Airfields";
    };
} // namespace UKControllerPlugin::MissedApproach
