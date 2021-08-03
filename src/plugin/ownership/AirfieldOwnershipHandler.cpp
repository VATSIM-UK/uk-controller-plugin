#include "pch/pch.h"
#include "ownership/AirfieldOwnershipHandler.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionParser.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "message/UserMessager.h"
#include "ownership/AirfieldOwnerQueryMessage.h"
#include "ownership/AirfieldsOwnedQueryMessage.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionParser;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldOwnerQueryMessage;
using UKControllerPlugin::Ownership::AirfieldsOwnedQueryMessage;

namespace UKControllerPlugin {
    namespace Ownership {

        AirfieldOwnershipHandler::AirfieldOwnershipHandler(
            UKControllerPlugin::Ownership::AirfieldOwnershipManager & airfieldOwnership,
            UserMessager & userMessager
        )
            : airfieldOwnership(airfieldOwnership),
            userMessager(userMessager)
        {

        }

        /*
            Loops through each of the airfields in the topdown order and refresh who owns them.
        */
        void AirfieldOwnershipHandler::ProcessAffectedAirfields(const ControllerPosition & controller)
        {
            std::vector<std::string> topDown = controller.GetTopdown();
            for (std::vector<std::string>::iterator it = topDown.begin(); it != topDown.end(); ++it) {
                this->airfieldOwnership.RefreshOwner(*it);
            }
        }

        void AirfieldOwnershipHandler::ActiveCallsignAdded(const ActiveCallsign& callsign, bool userCallsign)
        {
            // Refresh the top-down responsibilities for affected airfields
            this->ProcessAffectedAirfields(callsign.GetNormalisedPosition());
        }

        void AirfieldOwnershipHandler::ActiveCallsignRemoved(const ActiveCallsign& callsign, bool userCallsign)
        {
            // Refresh the top-down responsibilities for affected airfields
            this->ProcessAffectedAirfields(callsign.GetNormalisedPosition());
        }

        void AirfieldOwnershipHandler::CallsignsFlushed(void)
        {
            this->airfieldOwnership.Flush();
        }

        /*
            Processes commands to query airfield ownership.
        */
        bool AirfieldOwnershipHandler::ProcessCommand(std::string command)
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
    }  // namespace Ownership
}   // namespace UKControllerPlugin
