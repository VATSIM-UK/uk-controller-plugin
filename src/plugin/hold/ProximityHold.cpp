#include "ProximityHold.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Hold {

    ProximityHold::ProximityHold(std::string callsign, std::string navaid)
        : callsign(std::move(callsign)), navaid(std::move(navaid))
    {
    }

    ProximityHold::ProximityHold(
        std::string callsign, std::string navaid, std::chrono::system_clock::time_point enteredAt)
        : callsign(std::move(callsign)), navaid(std::move(navaid)), enteredAt(std::move(enteredAt))
    {
    }

    void ProximityHold::Enter()
    {
        this->enteredAt = Time::TimeNow();
    }

    auto ProximityHold::HasEntered() const -> bool
    {
        return this->enteredAt != NOT_ENTERED;
    }

    auto ProximityHold::Callsign() const -> const std::string&
    {
        return callsign;
    }

    auto ProximityHold::Navaid() const -> const std::string&
    {
        return navaid;
    }

    auto ProximityHold::EnteredAt() const -> const std::chrono::system_clock::time_point&
    {
        return enteredAt;
    }
} // namespace UKControllerPlugin::Hold
