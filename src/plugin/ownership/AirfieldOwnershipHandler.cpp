#include "AirfieldOwnerQueryMessage.h"
#include "AirfieldOwnershipHandler.h"
#include "AirfieldOwnershipManager.h"
#include "AirfieldServiceProviderCollection.h"
#include "AirfieldsOwnedQueryMessage.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionParser.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "message/UserMessager.h"
#include "ownership/ServiceProvision.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionParser;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldOwnerQueryMessage;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Ownership::AirfieldsOwnedQueryMessage;

namespace UKControllerPlugin::Ownership {

    AirfieldOwnershipHandler::AirfieldOwnershipHandler(
        UKControllerPlugin::Ownership::AirfieldOwnershipManager& airfieldOwnership, UserMessager& userMessager)
        : airfieldOwnership(airfieldOwnership), userMessager(userMessager)
    {
    }

    /*
        Loops through each of the airfields in the topdown order and refresh who owns them.
    */
    void AirfieldOwnershipHandler::ProcessAffectedAirfields(const ControllerPosition& controller)
    {
        std::vector<std::string> topDown = controller.GetTopdown();
        for (auto it = topDown.begin(); it != topDown.end(); ++it) {
            this->airfieldOwnership.RefreshOwner(*it);
        }
    }

    void AirfieldOwnershipHandler::ActiveCallsignAdded(const ActiveCallsign& callsign)
    {
        // Refresh the top-down responsibilities for affected airfields
        this->ProcessAffectedAirfields(callsign.GetNormalisedPosition());
    }

    void AirfieldOwnershipHandler::ActiveCallsignRemoved(const ActiveCallsign& callsign)
    {
        // Refresh the top-down responsibilities for affected airfields
        this->ProcessAffectedAirfields(callsign.GetNormalisedPosition());
    }

    void AirfieldOwnershipHandler::CallsignsFlushed()
    {
        this->airfieldOwnership.Flush();
    }

    /*
        Processes commands to query airfield ownership.
    */
    auto AirfieldOwnershipHandler::ProcessCommand(std::string command) -> bool
    {
        std::regex ownerRegex(".ukcp owner ([A-Za-z]{4})");
        std::smatch ownerMatches;
        if (std::regex_search(command, ownerMatches, ownerRegex)) {
            auto owner = this->airfieldOwnership.GetProviders().DeliveryProviderForAirfield(ownerMatches[1]);
            if (!owner) {
                return true;
            }

            const auto active = owner->controller;
            this->userMessager.SendMessageToUser(
                AirfieldOwnerQueryMessage(ownerMatches[1], active->GetCallsign(), active->GetControllerName()));
            return true;
        }

        std::regex ownedRegex(".ukcp owned ([A-Za-z0-9_-]+)");
        std::smatch ownedMatches;
        if (std::regex_search(command, ownedMatches, ownedRegex)) {
            std::vector<AirfieldModel> airfields;
            for (const auto& airfield : this->airfieldOwnership.GetOwnedAirfields(ownedMatches[1])) {
                airfields.push_back(airfield.get());
            }

            this->userMessager.SendMessageToUser(AirfieldsOwnedQueryMessage(std::move(airfields), ownedMatches[1]));
            return true;
        }

        return false;
    }
} // namespace UKControllerPlugin::Ownership
