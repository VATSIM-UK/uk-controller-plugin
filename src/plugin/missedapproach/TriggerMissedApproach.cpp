#include "MissedApproach.h"
#include "MissedApproachAudioAlert.h"
#include "MissedApproachCollection.h"
#include "MissedApproachTriggeredMessage.h"
#include "TriggerMissedApproach.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "helper/HelperFunctions.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "time/ParseTimeStrings.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::MissedApproach {

    TriggerMissedApproach::TriggerMissedApproach(
        std::shared_ptr<MissedApproachCollection> missedApproaches,
        Windows::WinApiInterface& windowsApi,
        const Api::ApiInterface& api,
        const Ownership::AirfieldServiceProviderCollection& serviceProviders,
        std::shared_ptr<const MissedApproachAudioAlert> audioAlert,
        const Integration::OutboundIntegrationEventHandler& integrationEvents)
        : missedApproaches(std::move(missedApproaches)), windowsApi(windowsApi), api(api),
          serviceProviders(serviceProviders), audioAlert(std::move(audioAlert)), integrationEvents(integrationEvents)
    {
    }

    /**
     * This could be triggered async by external integrations
     */
    void TriggerMissedApproach::Trigger(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
        bool userConfirm,
        const std::function<void(void)>& success,
        const std::function<void(std::vector<std::string>)>& fail) const
    {
        if (!AircraftElegibleForMissedApproach(flightplan, radarTarget)) {
            fail({"Aircraft not eligible for missed approach"});
            return;
        }

        if (!this->UserCanTrigger(flightplan)) {
            fail({"User not authorised to trigger missed approach"});
            LogWarning("User tried to trigger missed approach, but is not authorised to do so");
            return;
        }

        auto callsign = flightplan.GetCallsign();
        if (AlreadyActive(callsign)) {
            fail({"Missed approach already active"});
            LogWarning("Tried to create missed approach but one is alread active");
            return;
        }

        if (userConfirm && !Confirm(callsign)) {
            fail({"User did not confirm missed approach"});
            LogInfo("User did not confirm missed approach");
            return;
        }

        this->TriggerMissedApproachInApi(callsign, success, fail);
    }

    auto TriggerMissedApproach::Confirm(const std::string& callsign) const -> bool
    {
        std::wstring title = HelperFunctions::ConvertToWideString(callsign) + L" Missed Approach";
        return this->windowsApi.OpenMessageBox(
                   L"Are you sure you want to trigger a missed approach?",
                   title.c_str(),
                   MB_YESNO | MB_ICONEXCLAMATION) == IDYES;
    }

    /**
     * Only users providing tower services and actually logged on to a Tower position can call for a missed
     * approach.
     */
    auto TriggerMissedApproach::UserCanTrigger(Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> bool
    {
        const auto serviceProviders = this->serviceProviders.GetServiceProviders(flightplan.GetDestination());
        return std::find_if(
                   serviceProviders.begin(),
                   serviceProviders.end(),
                   [](const std::shared_ptr<Ownership::ServiceProvision>& provision) {
                       return provision->serviceProvided == Ownership::ServiceType::Tower &&
                              provision->controller->GetIsUser() &&
                              provision->controller->GetNormalisedPosition().IsTower();
                   }) != serviceProviders.cend();
    }

    auto TriggerMissedApproach::ResponseValid(const nlohmann::json& responseData) -> bool
    {
        return responseData.is_object() && responseData.contains("id") && responseData.at("id").is_number_integer() &&
               responseData.contains("expires_at") && responseData.at("expires_at").is_string() &&
               Time::ParseTimeString(responseData.at("expires_at").get<std::string>()) != Time::invalidTime;
    }

    void TriggerMissedApproach::TriggerMissedApproachInApi(
        const std::string& callsign,
        const std::function<void(void)> success,
        const std::function<void(std::vector<std::string>)> fail) const
    {
        Async([this, callsign, success, fail]() {
            try {
                auto response = this->api.CreateMissedApproach(callsign);
                if (!ResponseValid(response)) {
                    fail({"Bad API response when creating missed approach"});
                    LogError("Invalid response from API when creating missed approach");
                    return;
                }

                // Create the missed approach and play the alert
                const auto missedApproach = std::make_shared<class MissedApproach>(
                    response.at("id").get<int>(),
                    callsign,
                    Time::ParseTimeString(response.at("expires_at").get<std::string>()),
                    true);
                this->missedApproaches->Add(missedApproach);
                this->audioAlert->Play(missedApproach);

                // Send the integration message
                this->integrationEvents.SendEvent(std::make_shared<MissedApproachTriggeredMessage>(
                    missedApproach->Callsign(), true, missedApproach->ExpiresAt()));

                success();
            } catch (Api::ApiException&) {
                fail({"API error when creating missed approach"});
                LogError("ApiException when creating missed approach");
            }
        });
    }

    auto TriggerMissedApproach::AlreadyActive(const std::string& callsign) const -> bool
    {
        auto existing = this->missedApproaches->Get(callsign);
        return existing != nullptr && !existing->IsExpired();
    }

    auto TriggerMissedApproach::AircraftElegibleForMissedApproach(
        Euroscope::EuroScopeCFlightPlanInterface& flightplan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
        -> bool
    {
        return flightplan.GetDistanceToDestination() < MAX_DISTANCE_FROM_DESTINATION &&
               radarTarget.GetGroundSpeed() > MIN_GROUNDSPEED && radarTarget.GetFlightLevel() < MAX_ALTITUDE;
    }
} // namespace UKControllerPlugin::MissedApproach
