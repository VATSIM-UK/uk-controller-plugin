#include "historytrail/AircraftHistoryTrail.h"

namespace UKControllerPlugin {
    namespace HistoryTrail {

        AircraftHistoryTrail::AircraftHistoryTrail(std::string callsign)
        {
            this->callsign = callsign;
        }

        /*
            Adds an item to the history trail. If the queue is already at the maximum size, we remove the oldest
            item before adding the new one - so the trail will never get bigger than this->maxSize.

            The new item goes to the front, whilst old items are popped off the back.
        */
        void AircraftHistoryTrail::AddItem(HistoryTrailPoint point)
        {
            this->trail.push_front(point);

            if (this->trail.size() > this->maxSize) {
                this->trail.pop_back();
            }
        }

        /*
            Returns the callsign associated with this history trail.
        */
        std::string AircraftHistoryTrail::GetCallsign(void) const
        {
            return this->callsign;
        }

        /*
            Returns a copy of the current history trail.
        */
        const std::deque<HistoryTrailPoint>& AircraftHistoryTrail::GetTrail(void) const
        {
            return this->trail;
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
