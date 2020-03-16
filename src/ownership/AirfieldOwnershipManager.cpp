#include "pch/stdafx.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsignCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Ownership {

        AirfieldOwnershipManager::AirfieldOwnershipManager(
            const AirfieldCollection & airfields,
            const ActiveCallsignCollection & activeCallsigns
        )
            : airfields(airfields),
            activeCallsigns(activeCallsigns),
            notFoundControllerPosition("", 199.998, "XXX", {}),
            notFoundCallsign("", "", this->notFoundControllerPosition)
        {

        }

        /*
            Returns true if an airfield is "owned" by the given controller, false otherwise.
        */
        bool AirfieldOwnershipManager::AirfieldOwnedBy(std::string icao, const ActiveCallsign & position) const
        {
            return this->AirfieldHasOwner(icao) && *this->ownershipMap.find(icao)->second == position;
        }

        /*
            Returns true if an airfield is owned by the user.
        */
        bool AirfieldOwnershipManager::AirfieldOwnedByUser(std::string icao) const
        {
            return this->AirfieldHasOwner(icao) &&
                this->activeCallsigns.UserHasCallsign() &&
                *this->ownershipMap.find(icao)->second == this->activeCallsigns.GetUserCallsign();
        }

        /*
            Returns true if the airfield has an owner.
        */
        bool AirfieldOwnershipManager::AirfieldHasOwner(std::string icao) const
        {
            return this->ownershipMap.count(icao) > 0;
        }

        /*
            Sets no owner on all airfields.
        */
        void AirfieldOwnershipManager::Flush(void)
        {
            this->ownershipMap.clear();
        }

        /*
            Returns the controller that owns a given airfield.
        */
        const ActiveCallsign & AirfieldOwnershipManager::GetOwner(std::string icao) const
        {
            if (!this->AirfieldHasOwner(icao)) {
                return this->notFoundCallsign;
            }

            return *this->ownershipMap.find(icao)->second;
        }

        /*
            Returns a set of the airfields owned by a given contrroller.
        */
        std::vector<AirfieldModel> AirfieldOwnershipManager::GetOwnedAirfields(std::string callsign) const
        {
            std::vector<AirfieldModel> ownedAirfields;

            if (!this->activeCallsigns.CallsignActive(callsign)) {
                return ownedAirfields;
            }

            for (
                std::map<std::string, std::unique_ptr<ActiveCallsign>>::const_iterator it = this->ownershipMap.cbegin();
                it != this->ownershipMap.cend();
                ++it
            ) {
                if (*it->second == this->activeCallsigns.GetCallsign(callsign)) {
                    ownedAirfields.push_back(this->airfields.FetchAirfieldByIcao(it->first));
                }
            }

            return ownedAirfields;
        }

        /*
            Updates the owner of a given airfield.
        */
        void AirfieldOwnershipManager::RefreshOwner(std::string icao)
        {
            std::vector<std::string> topdownOrder;
            try {
                topdownOrder = this->airfields.FetchAirfieldByIcao(icao).GetOwnershipPresedence();
            }
            catch (std::out_of_range) {
                // Nothing we can do if we can't find the airfield.
                return;
            }

            // Loop through the topdown order
            for (std::vector<std::string>::iterator it = topdownOrder.begin(); it != topdownOrder.end(); ++it) {

                // If nobody is covering the position, don't count it, otherwise, take the lead callsign
                if (!this->activeCallsigns.PositionActive(*it)) {
                    continue;
                }

                // Only log when positions have changed hands
                bool needsLog = this->ownershipMap.count(icao) == 0 ||
                    this->ownershipMap.at(icao)->GetCallsign() !=
                    this->activeCallsigns.GetLeadCallsignForPosition(*it).GetCallsign();

                this->ownershipMap[icao] = std::make_unique<ActiveCallsign>(
                    this->activeCallsigns.GetLeadCallsignForPosition(*it)
                );

                if (needsLog) {
                    LogInfo(
                        "Airfield " + icao + " is now managed by " +
                        this->ownershipMap.find(icao)->second->GetCallsign()
                    );
                }
                return;
            }

            // We can't find an owner, so set no owner.
            LogInfo("Airfield " + icao + " is no longer managed by any controller");
            this->ownershipMap.erase(icao);
        }
    }  // namespace Ownership
}  // namespace UKControllerPlugin
