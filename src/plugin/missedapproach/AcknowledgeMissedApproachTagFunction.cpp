#include "AcknowledgeMissedApproachTagFunction.h"
#include "MissedApproach.h"
#include "MissedApproachCollection.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "ownership/AirfieldServiceProviderCollection.h"

namespace UKControllerPlugin::MissedApproach {

    AcknowledgeMissedApproachTagFunction::AcknowledgeMissedApproachTagFunction(
        const MissedApproachCollection& missedApproaches,
        const Dialog::DialogManager& dialogManager,
        const Api::ApiInterface& api,
        const Ownership::AirfieldServiceProviderCollection& airfieldServiceProviders)
        : missedApproaches(missedApproaches), dialogManager(dialogManager), api(api),
          airfieldServiceProviders(airfieldServiceProviders)
    {
    }

    void AcknowledgeMissedApproachTagFunction::TriggerDialog(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        this->lastAirfield = flightplan.GetDestination();
        this->lastCallsign = flightplan.GetCallsign();

        if (!this->UserCanTrigger()) {
            return;
        }

        const auto missed = this->MissedApproachForCallsign();
        if (!missed) {
            return;
        }

        this->lastCallsign = flightplan.GetCallsign();
        this->dialogManager.OpenDialog(IDD_MISSED_APPROACH_ACKNOWLEDGE, reinterpret_cast<LPARAM>(this));
    }

    auto AcknowledgeMissedApproachTagFunction::MissedApproachForCallsign() const
        -> const std::shared_ptr<class MissedApproach>
    {
        return this->missedApproaches.FirstWhere([this](const std::shared_ptr<class MissedApproach>& missed) -> bool {
            return missed->Callsign() == this->lastCallsign && !missed->IsAcknowledged() && !missed->CreatedByUser() &&
                   !missed->IsExpired();
        });
    }

    void AcknowledgeMissedApproachTagFunction::Acknowledge(const std::string& remarks) const
    {
        if (!this->UserCanTrigger()) {
            return;
        }

        const auto missed = this->MissedApproachForCallsign();
        if (!missed) {
            return;
        }

        Async([this, missed, remarks]() {
            try {
                this->api.AcknowledgeMissedApproach(missed->Id(), remarks);
                missed->Acknowledge("User", remarks);
            } catch (Api::ApiException&) {
                LogError("Failed to acknowledge missed approach");
            }
        });
    }

    auto AcknowledgeMissedApproachTagFunction::UserCanTrigger() const -> bool
    {
        return this->airfieldServiceProviders.ServiceProvidedAtAirfield(
            this->lastAirfield, Ownership::ServiceType::FinalApproach);
    }
} // namespace UKControllerPlugin::MissedApproach
