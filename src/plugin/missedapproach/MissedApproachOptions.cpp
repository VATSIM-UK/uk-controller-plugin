#include "MissedApproachOptions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachOptions::MissedApproachOptions()
        : audioAlert(false), alertForCurrentUser(false), seviceProvisions(ServiceType::Invalid)
    {
    }

    auto MissedApproachOptions::AudioAlertForCurrentUser() const -> bool
    {
        return alertForCurrentUser;
    }

    auto MissedApproachOptions::ServiceProvisions() const -> Ownership::ServiceType
    {
        return seviceProvisions;
    }

    void MissedApproachOptions::SetAudioAlertForCurrentUser(bool audioAlertForCurrentUser)
    {
        this->alertForCurrentUser = audioAlertForCurrentUser;
    }

    void MissedApproachOptions::SetServiceProvisions(Ownership::ServiceType audioAlertServiceProvisions)
    {
        this->seviceProvisions = audioAlertServiceProvisions;
    }

    void MissedApproachOptions::SetAirfields(std::vector<std::string> airfields)
    {
        this->airfields = std::move(airfields);
    }

    auto MissedApproachOptions::Airfields() const -> const std::vector<std::string>&
    {
        return this->airfields;
    }

    void MissedApproachOptions::SetAudioAlert(bool audioAlert)
    {
        this->audioAlert = audioAlert;
    }

    auto MissedApproachOptions::AudioAlert() const -> bool
    {
        return this->audioAlert;
    }
} // namespace UKControllerPlugin::MissedApproach
