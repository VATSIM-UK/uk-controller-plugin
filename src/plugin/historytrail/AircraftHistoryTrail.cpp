#include "AircraftHistoryTrail.h"

namespace UKControllerPlugin::HistoryTrail {

    AircraftHistoryTrail::AircraftHistoryTrail(std::string callsign) : callsign(std::move(callsign))
    {
    }

    /*
        Adds an item to the history trail. If the queue is already at the maximum size, we remove the oldest
        item before adding the new one - so the trail will never get bigger than this->maxSize.

        The new item goes to the front, whilst old items are popped off the back.
    */
    void AircraftHistoryTrail::AddItem(const HistoryTrailPoint& point)
    {
        this->trail.push_back(point);

        if (this->trail.size() > this->maxSize && this->trail.size() > 0) {
            this->trail.erase(this->trail.cbegin());
        }
    }

    /*
        Returns the callsign associated with this history trail.
    */
    auto AircraftHistoryTrail::GetCallsign() const -> std::string
    {
        return this->callsign;
    }

    /*
        Returns a copy of the current history trail.
    */
    auto AircraftHistoryTrail::GetTrail() const -> const std::vector<HistoryTrailPoint>&
    {
        return this->trail;
    }
} // namespace UKControllerPlugin::HistoryTrail
