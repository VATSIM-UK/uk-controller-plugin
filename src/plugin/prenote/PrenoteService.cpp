#include "pch/pch.h"
#include "prenote/PrenoteEventHandler.h"
#include "prenote/AbstractPrenote.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "message/UserMessager.h"
#include "prenote/PrenoteUserMessage.h"
#include "prenote/PrenoteService.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteUserMessage;

namespace UKControllerPlugin {
    namespace Prenote {

        PrenoteService::PrenoteService(
            const AirfieldOwnershipManager & airfieldOwnership,
            const ActiveCallsignCollection & activeCallsigns,
            UserMessager & userMessager
        )
            : airfieldOwnership(airfieldOwnership), activeCallsigns(activeCallsigns), userMessager(userMessager)
        {

        }

        void PrenoteService::AddPrenote(std::unique_ptr<const AbstractPrenote> prenote)
        {
            this->prenotes.insert(std::move(prenote));
        }

        void PrenoteService::CancelPrenote(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->alreadyPrenoted.erase(flightPlan.GetCallsign());
        }

        size_t PrenoteService::CountPrenotes(void) const
        {
            return this->prenotes.size();
        }

        void PrenoteService::PrenoteNotify(
            const AbstractPrenote & prenote,
            const EuroScopeCFlightPlanInterface & flightplan
        ) {
            for (
                ControllerPositionHierarchy::const_iterator it = prenote.GetControllers().cbegin();
                it != prenote.GetControllers().cend();
                ++it
            ) {
                if (!activeCallsigns.PositionActive(it->get().GetCallsign())) {
                    continue;
                }

                // If we're just prenoting ourselves, stop.
                if (it->get() == this->activeCallsigns.GetUserCallsign().GetNormalisedPosition()) {
                    return;
                }

                // Notify the user
                LogInfo(
                    "Notified user of required prenote to " + it->get().GetCallsign() +
                        " about " + flightplan.GetCallsign()
                );
                this->userMessager.SendMessageToUser(
                    PrenoteUserMessage(
                        prenote,
                        this->activeCallsigns.GetLeadCallsignForPosition(it->get().GetCallsign()),
                        flightplan
                    )
                );

                return;
            }
        }

        void PrenoteService::SendPrenotes(EuroScopeCFlightPlanInterface & flightplan)
        {
            if (!this->airfieldOwnership.AirfieldOwnedByUser(flightplan.GetOrigin())) {
                return;
            }

            if (this->alreadyPrenoted.find(flightplan.GetCallsign()) != this->alreadyPrenoted.end()) {
                return;
            }

            for (
                std::set<std::unique_ptr<const AbstractPrenote>>::const_iterator it = this->prenotes.cbegin();
                it != this->prenotes.cend();
                ++it
            ) {
                if ((*it)->IsApplicable(flightplan)) {
                    this->PrenoteNotify(**it, flightplan);
                }
            }

            this->alreadyPrenoted.insert(flightplan.GetCallsign());
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
