#include "MissedApproachOptions.h"
#include "MissedApproachUserSettingHandler.h"
#include "euroscope/UserSetting.h"
#include "euroscope/UserSettingAwareCollection.h"

using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachUserSettingHandler::MissedApproachUserSettingHandler(
        std::shared_ptr<MissedApproachOptions> options,
        const Euroscope::UserSettingAwareCollection& userSettingsHandlers,
        Euroscope::UserSetting& userSettings)
        : options(std::move(options)), userSettingsHandlers(userSettingsHandlers), userSettings(userSettings)
    {
    }

    void MissedApproachUserSettingHandler::UserSettingsUpdated(Euroscope::UserSetting& userSettings)
    {
        // What service provisions to ping for
        auto settingsServiceProvision = userSettings.GetUnsignedIntegerEntry(
            this->SERVICE_PROVISION_ASR_KEY, static_cast<unsigned int>(ServiceType::Invalid));

        this->options->SetAudioAlertServiceProvisions(
            settingsServiceProvision >= static_cast<unsigned int>(ServiceType::Invalid)
                ? ServiceType::Invalid
                : static_cast<ServiceType>(settingsServiceProvision));

        // Should we ping for ourselves
        this->options->SetAudioAlertForCurrentUser(userSettings.GetBooleanEntry(this->CURRENT_USER_AUDIO_ASR_KEY));
    }

    void MissedApproachUserSettingHandler::SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser)
    {
        this->userSettings.Save(
            this->CURRENT_USER_AUDIO_ASR_KEY, this->CURRENT_USER_AUDIO_ASR_DESC, audioAlertForCurrentUser);
        this->options->SetAudioAlertForCurrentUser(audioAlertForCurrentUser);
        this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
    }

    void
    MissedApproachUserSettingHandler::SetAudioAlertServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions)
    {
        this->userSettings.Save(
            this->SERVICE_PROVISION_ASR_KEY,
            this->SERVICE_PROVISION_ASR_DESC,
            static_cast<unsigned int>(audioAlertServiceProvisions));
        this->options->SetAudioAlertServiceProvisions(audioAlertServiceProvisions);
        this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
    }
} // namespace UKControllerPlugin::MissedApproach
