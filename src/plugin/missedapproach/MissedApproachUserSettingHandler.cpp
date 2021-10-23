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
        // Do we play an audio alert
        this->options->SetAudioAlert(userSettings.GetBooleanEntry(this->AUDIO_KEY, false));

        // What service provisions to ping for
        auto settingsServiceProvision = userSettings.GetUnsignedIntegerEntry(
            this->SERVICE_PROVISION_KEY, static_cast<unsigned int>(ServiceType::Invalid));

        this->options->SetServiceProvisions(
            settingsServiceProvision >= static_cast<unsigned int>(ServiceType::Invalid)
                ? ServiceType::Invalid
                : static_cast<ServiceType>(settingsServiceProvision));

        // Should we ping for ourselves
        this->options->SetAudioAlertForCurrentUser(userSettings.GetBooleanEntry(this->CURRENT_USER_AUDIO_KEY));

        // Which airfields
        this->options->SetAirfields(userSettings.GetStringListEntry(this->AIRFIELDS_KEY, {}));
    }

    void MissedApproachUserSettingHandler::SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser)
    {
        this->userSettings.Save(this->CURRENT_USER_AUDIO_KEY, this->CURRENT_USER_AUDIO_DESC, audioAlertForCurrentUser);
        this->options->SetAudioAlertForCurrentUser(audioAlertForCurrentUser);
        this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
    }

    void MissedApproachUserSettingHandler::SetServiceProvisions(Ownership::ServiceType serviceProvisions)
    {
        this->userSettings.Save(
            this->SERVICE_PROVISION_KEY, this->SERVICE_PROVISION_DESC, static_cast<unsigned int>(serviceProvisions));
        this->options->SetServiceProvisions(serviceProvisions);
        this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
    }

    auto MissedApproachUserSettingHandler::GetOptions() const -> const std::shared_ptr<const MissedApproachOptions>
    {
        return this->options;
    }

    void MissedApproachUserSettingHandler::SetAudioAlert(bool audioAlert)
    {
        this->userSettings.Save(this->AUDIO_KEY, this->AUDIO_DESC, audioAlert);
        this->options->SetAudioAlert(audioAlert);
        this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
    }

    void MissedApproachUserSettingHandler::SetAirfields(std::vector<std::string> airfields)
    {
        this->userSettings.Save(this->AIRFIELDS_KEY, this->AIRFIELDS_DESC, airfields);
        this->options->SetAirfields(std::move(airfields));
        this->userSettingsHandlers.UserSettingsUpdateEvent(this->userSettings);
    }
} // namespace UKControllerPlugin::MissedApproach
