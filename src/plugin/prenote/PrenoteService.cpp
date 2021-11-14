#include "AbstractPrenote.h"
#include "PrenoteEventHandler.h"
#include "PrenoteService.h"
#include "PrenoteUserMessage.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "message/UserMessager.h"
#include "ownership/AirfieldServiceProviderCollection.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Prenote::PrenoteUserMessage;

namespace UKControllerPlugin::Prenote {

    PrenoteService::PrenoteService(
        const AirfieldServiceProviderCollection& airfieldOwnership,
        const ActiveCallsignCollection& activeCallsigns,
        UserMessager& userMessager)
        : activeCallsigns(activeCallsigns), airfieldOwnership(airfieldOwnership), userMessager(userMessager)
    {
    }

    PrenoteService::~PrenoteService() = default;
    PrenoteService::PrenoteService(PrenoteService&&) noexcept = default;

    void PrenoteService::AddPrenote(std::unique_ptr<const AbstractPrenote> prenote)
    {
        this->prenotes.insert(std::move(prenote));
    }

    void PrenoteService::CancelPrenote(EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->alreadyPrenoted.erase(flightPlan.GetCallsign());
    }

    auto PrenoteService::CountPrenotes() const -> size_t
    {
        return this->prenotes.size();
    }

    void PrenoteService::PrenoteNotify(const AbstractPrenote& prenote, const EuroScopeCFlightPlanInterface& flightplan)
    {
        for (const auto& position : prenote.GetControllers()) {
            if (!activeCallsigns.PositionActive(position->GetCallsign())) {
                continue;
            }

            // If we're just prenoting ourselves, stop.
            if (*position == this->activeCallsigns.GetUserCallsign().GetNormalisedPosition()) {
                return;
            }

            // Notify the user
            LogInfo(
                "Notified user of required prenote to " + position->GetCallsign() + " about " +
                flightplan.GetCallsign());
            this->userMessager.SendMessageToUser(PrenoteUserMessage(
                prenote, this->activeCallsigns.GetLeadCallsignForPosition(position->GetCallsign()), flightplan));

            return;
        }
    }

    void PrenoteService::SendPrenotes(EuroScopeCFlightPlanInterface& flightplan)
    {
        if (!this->airfieldOwnership.DeliveryControlProvidedByUser(flightplan.GetOrigin())) {
            return;
        }

        if (this->alreadyPrenoted.find(flightplan.GetCallsign()) != this->alreadyPrenoted.end()) {
            return;
        }

        for (const auto& prenote : this->prenotes) {
            if (prenote->IsApplicable(flightplan)) {
                this->PrenoteNotify(*prenote, flightplan);
            }
        }

        this->alreadyPrenoted.insert(flightplan.GetCallsign());
    }
} // namespace UKControllerPlugin::Prenote
