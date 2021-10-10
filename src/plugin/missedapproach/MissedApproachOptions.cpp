#include "MissedApproachOptions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachOptions::MissedApproachOptions()
        : audioAlertForCurrentUser(false), audioAlertServiceProvisions(ServiceType::Invalid)
    {
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
    }

    void MissedApproachOptions::SetAudioAlertServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions)
    {
        this->audioAlertServiceProvisions = audioAlertServiceProvisions;
    }
} // namespace UKControllerPlugin::MissedApproach
