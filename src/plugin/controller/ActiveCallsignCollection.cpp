#include "ActiveCallsign.h"
#include "ActiveCallsignCollection.h"
#include "ControllerPosition.h"
#include "euroscope/EuroScopeCControllerInterface.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;

namespace UKControllerPlugin::Controller {

    ActiveCallsignCollection::ActiveCallsignCollection()
    {
    }

    /*
        Inserts the callsign into the position set and also stores the iterator
        pointing to it in the callsign list for easy access.
    */
    void ActiveCallsignCollection::AddCallsign(const ActiveCallsign& controller)
    {
        if (this->CallsignActive(controller.GetCallsign())) {
            throw std::invalid_argument("Controller " + controller.GetCallsign() + " already active.");
        }

        this->activeCallsigns[controller.GetCallsign()] =
            this->activePositions[controller.GetNormalisedPosition().GetCallsign()].insert(controller).first;

        for (auto it = this->handlers.cbegin(); it != this->handlers.cend(); ++it) {
            (*it)->ActiveCallsignAdded(controller);
        }
    }

    /*
        Add a callsign for the "current user".
    */
    void ActiveCallsignCollection::AddUserCallsign(const ActiveCallsign& controller)
    {
        if (this->CallsignActive(controller.GetCallsign())) {
            throw std::invalid_argument("Callsign is already active.");
        }

        if (this->userActive) {
            this->RemoveCallsign(*this->userCallsign);
        }

        this->userCallsign =
            this->activePositions[controller.GetNormalisedPosition().GetCallsign()].insert(controller).first;
        this->activeCallsigns[controller.GetCallsign()] = this->userCallsign;
        this->userActive = true;

        for (auto it = this->handlers.cbegin(); it != this->handlers.cend(); ++it) {
            (*it)->ActiveCallsignAdded(controller);
        }
    }

    /*
        Returns true if a callsign is known to be active, false otherwise.
    */
    auto ActiveCallsignCollection::CallsignActive(const std::string& callsign) const -> bool
    {
        return this->activeCallsigns.count(callsign) != 0;
    }

    /*
        Flushes the entire collection. Sad times.
    */
    void ActiveCallsignCollection::Flush()
    {
        this->activeCallsigns.clear();
        this->activePositions.clear();
        this->userActive = false;
        for (auto it = this->handlers.cbegin(); it != this->handlers.cend(); ++it) {
            (*it)->CallsignsFlushed();
        }
    }

    auto ActiveCallsignCollection::GetNumberActiveCallsigns() const -> int
    {
        return this->activeCallsigns.size();
    }

    auto ActiveCallsignCollection::GetNumberActivePositions() const -> int
    {
        return this->activePositions.size();
    }

    /*
        Returns a particular active callsign.
    */
    auto ActiveCallsignCollection::GetCallsign(const std::string& callsign) const -> ActiveCallsign
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
    auto ActiveCallsignCollection::GetLeadCallsignForPosition(const std::string& normalisedCallsign) const
        -> ActiveCallsign
    {
        if (!this->PositionActive(normalisedCallsign)) {
            throw std::out_of_range("Position not found");
        }

        return *this->activePositions.find(normalisedCallsign)->second.begin();
    }

    /*
        Returns the users active callsign. Throws exception if not found.
    */
    auto ActiveCallsignCollection::GetUserCallsign() const -> ActiveCallsign
    {
        if (!this->userActive) {
            throw std::out_of_range("User has no callsign.");
        }

        return *this->userCallsign;
    }

    /*
        Returns whether or not there is an active callsign for a given controller position.
    */
    auto ActiveCallsignCollection::PositionActive(const std::string& normalisedCallsign) const -> bool
    {
        return this->activePositions.find(normalisedCallsign) != this->activePositions.end() &&
               !this->activePositions.find(normalisedCallsign)->second.empty();
    }

    /*
        Removes a callsign from the active lists.
    */
    void ActiveCallsignCollection::RemoveCallsign(const ActiveCallsign& controller)
    {
        auto callsign = this->activeCallsigns.find(controller.GetCallsign());

        if (callsign == this->activeCallsigns.end()) {
            LogError("Tried to remove inactive callsign " + controller.GetCallsign());
            return;
        }

        // If they're the current user, mark inactive.
        bool isUser = this->userActive && *callsign->second == *this->userCallsign;
        if (isUser) {
            this->userActive = false;
        }

        this->activePositions.find(controller.GetNormalisedPosition().GetCallsign())->second.erase(callsign->second);
        this->activeCallsigns.erase(callsign);

        for (auto it = this->handlers.cbegin(); it != this->handlers.cend(); ++it) {
            (*it)->ActiveCallsignRemoved(controller);
        }
    }

    /*
        Returns true if the user has an active callsign.
    */
    auto ActiveCallsignCollection::UserHasCallsign() const -> bool
    {
        return this->userActive;
    }

    void ActiveCallsignCollection::AddHandler(const std::shared_ptr<ActiveCallsignEventHandlerInterface>& handler)
    {
        if (std::find(this->handlers.cbegin(), this->handlers.cend(), handler) != this->handlers.cend()) {
            LogWarning("Duplicate ActiveCallsignEventHandler detected");
            return;
        }

        this->handlers.push_back(handler);
    }

    auto ActiveCallsignCollection::CountHandlers() const -> size_t
    {
        return this->handlers.size();
    }
} // namespace UKControllerPlugin::Controller
