#include "PrenoteEventHandler.h"
#include "PrenoteService.h"
#include "PublishedPrenote.h"
#include "PublishedPrenoteMapper.h"
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
        const PublishedPrenoteMapper& prenoteMapper,
        const AirfieldServiceProviderCollection& airfieldOwnership,
        const ActiveCallsignCollection& activeCallsigns,
        UserMessager& userMessager)
        : prenoteMapper(prenoteMapper), activeCallsigns(activeCallsigns), airfieldOwnership(airfieldOwnership),
          userMessager(userMessager)
    {
    }

    PrenoteService::~PrenoteService() = default;
    PrenoteService::PrenoteService(PrenoteService&&) noexcept = default;

    void PrenoteService::CancelPrenote(EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->alreadyPrenoted.erase(flightPlan.GetCallsign());
    }

    void PrenoteService::SendPrenotes(EuroScopeCFlightPlanInterface& flightplan)
    {
        if (!this->airfieldOwnership.DeliveryControlProvidedByUser(flightplan.GetOrigin())) {
            return;
        }

        if (this->alreadyPrenoted.find(flightplan.GetCallsign()) != this->alreadyPrenoted.end()) {
            return;
        }

        std::set<int> controllersAlreadyPrenoted;
        for (const auto& prenote : this->prenoteMapper.MapForFlightplan(flightplan)) {
            for (const auto& controller : *prenote->controllers) {
                // Position not active, cannot prenote.
                if (!activeCallsigns.PositionActive(controller->GetCallsign())) {
                    continue;
                }

                // If we're just prenoting ourselves, stop.
                const auto leadCallsign = activeCallsigns.GetLeadCallsignForPosition(controller->GetCallsign());
                if (leadCallsign.GetIsUser()) {
                    break;
                }

                // This controllers already been prenoted for this aircraft, stop.
                if (controllersAlreadyPrenoted.find(controller->GetId()) != controllersAlreadyPrenoted.end()) {
                    break;
                }

                LogInfo(
                    "Notified user of required prenote to " + controller->GetCallsign() + " about " +
                    flightplan.GetCallsign());
                this->userMessager.SendMessageToUser(PrenoteUserMessage(leadCallsign, flightplan));
                controllersAlreadyPrenoted.insert(controller->GetId());
                break;
            }
        }

        this->alreadyPrenoted.insert(flightplan.GetCallsign());
    }
} // namespace UKControllerPlugin::Prenote
