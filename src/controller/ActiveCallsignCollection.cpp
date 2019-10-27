#include "pch/stdafx.h"
#include "controller/ActiveCallsignCollection.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Controller {


        ActiveCallsignCollection::ActiveCallsignCollection(void)
        {

        }

        /*
            Inserts the callsign into the position set and also stores the iterator
            pointing to it in the callsign list for easy access.
        */
        void ActiveCallsignCollection::AddCallsign(ActiveCallsign controller)
        {
            if (this->CallsignActive(controller.GetCallsign())) {
                throw std::invalid_argument("Controller " + controller.GetCallsign() + " already active.");
            }

            this->activeCallsigns[controller.GetCallsign()] =
                this->activePositions[controller.GetNormalisedPosition().GetCallsign()]
                .insert(controller).first;
        }

        /*
            Add a callsign for the "current user".
        */
        void ActiveCallsignCollection::AddUserCallsign(ActiveCallsign controller)
        {
            if (this->CallsignActive(controller.GetCallsign())) {
                throw std::invalid_argument("Callsign is already active.");
            }

            if (this->userActive) {
                this->RemoveCallsign(*this->userCallsign);
            }

            this->userCallsign = this->activePositions[controller.GetNormalisedPosition().GetCallsign()]
                .insert(controller).first;
            this->activeCallsigns[controller.GetCallsign()] = this->userCallsign;
            this->userActive = true;
        }

        /*
            Returns true if a callsign is known to be active, false otherwise.
        */
        bool ActiveCallsignCollection::CallsignActive(std::string callsign) const
        {
            return this->activeCallsigns.count(callsign) != 0;
        }

        /*
            Flushes the entire collection. Sad times.
        */
        void ActiveCallsignCollection::Flush(void)
        {
            this->activeCallsigns.clear();
            this->activePositions.clear();
            this->userActive = false;
        }

        int ActiveCallsignCollection::GetNumberActiveCallsigns() const
        {
            return this->activeCallsigns.size();
        }

        int ActiveCallsignCollection::GetNumberActivePositions() const
        {
            return this->activePositions.size();
        }

        /*
            Returns a particular active callsign.
        */
        ActiveCallsign ActiveCallsignCollection::GetCallsign(std::string callsign) const
        {
            if (!this->CallsignActive(callsign)) {
                throw std::out_of_range("Callsign not found");
            }
#
            return *this->activeCallsigns.find(callsign)->second;
        }

        /*
            Returns the "lead" callsign for a given position.
        */
        ActiveCallsign ActiveCallsignCollection::GetLeadCallsignForPosition(std::string normalisedCallsign) const
        {
            if (!this->PositionActive(normalisedCallsign)) {
                throw std::out_of_range("Position not found");
            }

            return *this->activePositions.find(normalisedCallsign)->second.begin();
        }

        /*
            Returns the users active callsign. Throws exception if not found.
        */
        ActiveCallsign ActiveCallsignCollection::GetUserCallsign(void) const
        {
            if (!this->userActive) {
                throw std::out_of_range("User has no callsign.");
            }

            return *this->userCallsign;
        }

        /*
            Returns whether or not there is an active callsign for a given controller position.
        */
        bool ActiveCallsignCollection::PositionActive(std::string normalisedCallsign) const
        {
            return this->activePositions.find(normalisedCallsign) != this->activePositions.end() &&
                this->activePositions.find(normalisedCallsign)->second.size() != 0;
        }

        /*
            Removes a callsign from the active lists.
        */
        void ActiveCallsignCollection::RemoveCallsign(ActiveCallsign controller)
        {
            std::map<std::string, std::set<ActiveCallsign>::iterator>::iterator callsign = this->activeCallsigns.find(
                controller.GetCallsign()
            );

            if (callsign == this->activeCallsigns.end()) {
                LogError("Tried to remove inactive callsign " + controller.GetCallsign());
                return;
            }

            // If they're the current user, mark inactive.
            if (this->userActive && *callsign->second == *this->userCallsign) {
                this->userActive = false;
            }

            this->activePositions.find(
                controller.GetNormalisedPosition().GetCallsign()
            )->second.erase(callsign->second);
            this->activeCallsigns.erase(callsign);
        }

        /*
            Returns true if the user has an active callsign.
        */
        bool ActiveCallsignCollection::UserHasCallsign(void) const
        {
            return this->userActive;
        }

    }  // namespace Controller
}  // namespace UKControllerPlugin
