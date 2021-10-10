#include "MissedApproachOptions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachOptions::MissedApproachOptions(UserSetting& userSettings)
        : userSettings(userSettings), audioAlertForCurrentUser(false), audioAlertServiceProvisions(ServiceType::Invalid)
    {
    }

    void MissedApproachOptions::UserSettingsUpdated(UserSetting& userSettings)
    {
        // What service provisions to ping for
        auto settingsServiceProvision = userSettings.GetUnsignedIntegerEntry(
            this->SERVICE_PROVISION_ASR_KEY, static_cast<unsigned int>(ServiceType::Invalid));

        this->audioAlertServiceProvisions = settingsServiceProvision >= static_cast<unsigned int>(ServiceType::Invalid)
                                                ? ServiceType::Invalid
                                                : static_cast<ServiceType>(settingsServiceProvision);

        // Should we ping for ourselves
        this->audioAlertForCurrentUser = userSettings.GetBooleanEntry(this->CURRENT_USER_AUDIO_ASR_KEY);
    }

    auto MissedApproachOptions::AudioAlertForCurrentUser() const -> bool
    {
        return audioAlertForCurrentUser;
    }

    auto MissedApproachOptions::AudioAlertServiceProvisions() const -> Ownership::ServiceType
    {
        return audioAlertServiceProvisions;
    }

    void MissedApproachOptions::SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser)
    {
        this->audioAlertForCurrentUser = audioAlertForCurrentUser;
        this->userSettings.Save(
            this->CURRENT_USER_AUDIO_ASR_KEY, this->CURRENT_USER_AUDIO_ASR_DESC, this->audioAlertForCurrentUser);
    }

    void MissedApproachOptions::SetAudioAlertServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions)
    {
        this->audioAlertServiceProvisions = audioAlertServiceProvisions;
        this->userSettings.Save(
            this->SERVICE_PROVISION_ASR_KEY,
            this->SERVICE_PROVISION_ASR_DESC,
            static_cast<unsigned int>(this->audioAlertServiceProvisions));
    }
} // namespace UKControllerPlugin::MissedApproach
