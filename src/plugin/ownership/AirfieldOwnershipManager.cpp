#include "AirfieldOwnershipManager.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin::Ownership {

    AirfieldOwnershipManager::AirfieldOwnershipManager(
        const AirfieldCollection& airfields, const ActiveCallsignCollection& activeCallsigns)
        : airfields(airfields), activeCallsigns(activeCallsigns),
          notFoundControllerPosition(
              std::make_unique<ControllerPosition>(-1, "", INVALID_FREQUENCY, std::vector<std::string>{}, true, false)),
          notFoundCallsign(std::make_unique<ActiveCallsign>("", "", *this->notFoundControllerPosition))
    {
    }

    /*
        Returns true if an airfield is "owned" by the given controller, false otherwise.
    */
    auto AirfieldOwnershipManager::AirfieldOwnedBy(const std::string& icao, const ActiveCallsign& position) const -> bool
    {
        return this->AirfieldHasOwner(icao) && *this->ownershipMap.find(icao)->second == position;
    }

    /*
        Returns true if an airfield is owned by the user.
    */
    auto AirfieldOwnershipManager::AirfieldOwnedByUser(const std::string& icao) const -> bool
    {
        return this->AirfieldHasOwner(icao) && this->activeCallsigns.UserHasCallsign() &&
               *this->ownershipMap.find(icao)->second == this->activeCallsigns.GetUserCallsign();
    }

    /*
        Returns true if the airfield has an owner.
    */
    auto AirfieldOwnershipManager::AirfieldHasOwner(const std::string& icao) const -> bool
    {
        return this->ownershipMap.count(icao) > 0;
    }

    /*
        Sets no owner on all airfields.
    */
    void AirfieldOwnershipManager::Flush()
    {
        this->ownershipMap.clear();
    }

    /*
        Returns the controller that owns a given airfield.
    */
    auto AirfieldOwnershipManager::GetOwner(const std::string& icao) const -> const ActiveCallsign&
    {
        if (!this->AirfieldHasOwner(icao)) {
            return *this->notFoundCallsign;
        }

        return *this->ownershipMap.find(icao)->second;
    }

    /*
        Returns a set of the airfields owned by a given contrroller.
    */
    auto AirfieldOwnershipManager::GetOwnedAirfields(const std::string& callsign) const
        -> std::vector<std::reference_wrapper<const AirfieldModel>>
    {
        std::vector<std::reference_wrapper<const AirfieldModel>> ownedAirfields;

        if (!this->activeCallsigns.CallsignActive(callsign)) {
            return ownedAirfields;
        }

        for (auto it = this->ownershipMap.cbegin(); it != this->ownershipMap.cend(); ++it) {
            if (*it->second == this->activeCallsigns.GetCallsign(callsign)) {
                ownedAirfields.emplace_back(this->airfields.FetchAirfieldByIcao(it->first));
            }
        }

        return ownedAirfields;
    }

    /*
        Updates the owner of a given airfield.
    */
    void AirfieldOwnershipManager::RefreshOwner(const std::string& icao)
    {
        std::vector<std::string> topdownOrder;
        try {
            topdownOrder = this->airfields.FetchAirfieldByIcao(icao).GetOwnershipPresedence();
        } catch (std::out_of_range&) {
            // Nothing we can do if we can't find the airfield.
            return;
        }

        // Loop through the topdown order
        for (auto it = topdownOrder.begin(); it != topdownOrder.end(); ++it) {

            // If nobody is covering the position, don't count it, otherwise, take the lead callsign
            if (!this->activeCallsigns.PositionActive(*it)) {
                continue;
            }

            // Only log when positions have changed hands
            bool needsLog = this->ownershipMap.count(icao) == 0 ||
                            this->ownershipMap.at(icao)->GetCallsign() !=
                                this->activeCallsigns.GetLeadCallsignForPosition(*it).GetCallsign();

            this->ownershipMap[icao] =
                std::make_unique<ActiveCallsign>(this->activeCallsigns.GetLeadCallsignForPosition(*it));

            if (needsLog) {
                LogInfo(
                    "Airfield " + icao + " is now managed by " + this->ownershipMap.find(icao)->second->GetCallsign());
            }
            return;
        }

        // We can't find an owner, so set no owner.
        LogInfo("Airfield " + icao + " is no longer managed by any controller");
        this->ownershipMap.erase(icao);
    }
} // namespace UKControllerPlugin::Ownership
