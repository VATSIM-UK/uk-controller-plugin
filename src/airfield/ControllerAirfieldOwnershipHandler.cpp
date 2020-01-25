#include "pch/stdafx.h"
#include "airfield/ControllerAirfieldOwnershipHandler.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionParser.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "message/UserMessager.h"
#include "controller/AirfieldOwnerQueryMessage.h"
#include "controller/AirfieldsOwnedQueryMessage.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionParser;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::EventHandler::MassEvent;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Controller::AirfieldOwnerQueryMessage;
using UKControllerPlugin::Controller::AirfieldsOwnedQueryMessage;

namespace UKControllerPlugin {
    namespace Airfield {

        ControllerAirfieldOwnershipHandler::ControllerAirfieldOwnershipHandler(
            const ControllerPositionCollection & controllers,
            UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership,
            ActiveCallsignCollection & activeCallsigns,
            const MassEvent massEventHandler,
            UserMessager & userMessager
        )
            : activeCallsigns(activeCallsigns),
            airfieldOwnership(airfieldOwnership),
            massEventHandler(massEventHandler),
            controllers(controllers),
            userMessager(userMessager)
        {

        }

        /*
            Called when a controller disconnects from the network - remove from the active callsign list.
        */
        void ControllerAirfieldOwnershipHandler::ControllerDisconnectEvent(EuroScopeCControllerInterface & controller)
        {
            // If we don't have the callsign, nothing to do.
            if (!this->activeCallsigns.CallsignActive(controller.GetCallsign())) {
                return;
            }

            ActiveCallsign active = this->activeCallsigns.GetCallsign(controller.GetCallsign());
            LogInfo(controller.GetCallsign() + " has disconnected or unset their primary frequency");
            this->activeCallsigns.RemoveCallsign(active);

            // Refresh the top-down responsibilities for affected airfields
            this->ProcessAffectedAirfields(active.GetNormalisedPosition());
        }

        /*
            Called when a controller update happens.
        */
        void ControllerAirfieldOwnershipHandler::ControllerUpdateEvent(EuroScopeCControllerInterface & controller)
        {
            bool callsignActive = this->activeCallsigns.CallsignActive(controller.GetCallsign());
            bool frequencyActive = controller.HasActiveFrequency();

            // If it's already an active callsign with an active frequency, do nothing.
            if (callsignActive && frequencyActive) {
                return;
            }

            // If they're a registered callsign, but they've not got an active frequency, they must have unset primary.
            if (callsignActive && !frequencyActive) {
                this->ControllerDisconnectEvent(controller);
                return;
            }

            // They don't have an active frequency, which probably means they've just logged in.
            if (!frequencyActive) {
                return;
            }

            // Perform a match based on frequency and facility to find the canonical position
            ControllerPositionParser parser;
            try {
                this->SetupPosition(controller, this->controllers.FetchPositionByFacilityAndFrequency(
                    parser.ParseFacilityFromCallsign(controller.GetCallsign()),
                    controller.GetFrequency()
                ));
            }
            catch (std::out_of_range) {
                // No position at all, so nothing we can do.
                return;
            }
        }

        /*
            Loops through each of the airfields in the topdown order and refresh who owns them.
        */
        void ControllerAirfieldOwnershipHandler::ProcessAffectedAirfields(const ControllerPosition & controller)
        {
            std::vector<std::string> topDown = controller.GetTopdown();
            for (std::vector<std::string>::iterator it = topDown.begin(); it != topDown.end(); ++it) {
                this->airfieldOwnership.RefreshOwner(*it);
            }
        }

        /*
            Processes commands to query airfield ownership.
        */
        bool ControllerAirfieldOwnershipHandler::ProcessCommand(std::string command)
        {
            std::regex ownerRegex(".ukcp owner ([A-Za-z]{4})");
            std::smatch ownerMatches;
            if (std::regex_search(command, ownerMatches, ownerRegex)) {
                ActiveCallsign active = this->airfieldOwnership.GetOwner(ownerMatches[1]);
                this->userMessager.SendMessageToUser(AirfieldOwnerQueryMessage(
                    ownerMatches[1],
                    active.GetCallsign(),
                    active.GetControllerName()
                ));
                return true;
            }

            std::regex ownedRegex(".ukcp owned ([A-Za-z0-9_-]+)");
            std::smatch ownedMatches;
            if (std::regex_search(command, ownedMatches, ownedRegex)) {
                this->userMessager.SendMessageToUser(AirfieldsOwnedQueryMessage(
                    this->airfieldOwnership.GetOwnedAirfields(ownedMatches[1]),
                    ownedMatches[1]
                ));
                return true;
            }

            return false;
        }

        /*
            Called when the user is deemed to have logged out of Euroscope. Clear the entire online position
            and airfield ownership caches.
        */
        void ControllerAirfieldOwnershipHandler::SelfDisconnectEvent(void)
        {
            this->activeCallsigns.Flush();
            this->airfieldOwnership.Flush();
        }

        /*
            Sets up a new controller position.
        */
        void ControllerAirfieldOwnershipHandler::SetupPosition(
            EuroScopeCControllerInterface & callsign,
            const ControllerPosition & matchedPos
        ) {
            // Add to the active callsign collection.
            if (callsign.IsCurrentUser()) {
                LogInfo(
                    "The current user, with callsign " + callsign.GetCallsign() +
                    ", has been marked as active, covering " + matchedPos.GetCallsign()
                );
                this->activeCallsigns.AddUserCallsign(
                    ActiveCallsign(callsign.GetCallsign(), callsign.GetControllerName(), matchedPos)
                );
            } else {
                LogInfo(
                    callsign.GetCallsign() + " has been marked as active, covering " + matchedPos.GetCallsign()
                );
                this->activeCallsigns.AddCallsign(
                    ActiveCallsign(callsign.GetCallsign(), callsign.GetControllerName(), matchedPos)
                );
            }

            // Work out who owns what airfields.
            this->ProcessAffectedAirfields(matchedPos);

            // If it's the current user logging on, update squawks, initial altitudes etc.
            if (callsign.IsCurrentUser()) {
                this->massEventHandler.SetAllInitialAltitudes();
                this->massEventHandler.SetAllSquawks();
            }
        }
    }  // namespace Airfield
}   // namespace UKControllerPlugin
