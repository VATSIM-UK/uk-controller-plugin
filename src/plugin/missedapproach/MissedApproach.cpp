#include "MissedApproach.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::MissedApproach {
    MissedApproach::MissedApproach(std::string callsign, std::chrono::system_clock::time_point expiresAt)
        : callsign(std::move(callsign)), expiresAt(expiresAt)
    {
    }

    auto MissedApproach::Callsign() const -> const std::string&
    {
        return callsign;
    }

    auto MissedApproach::ExpiresAt() const -> const std::chrono::system_clock::time_point&
    {
        return expiresAt;
    }

    auto MissedApproach::IsExpired() const -> bool
    {
        return expiresAt < Time::TimeNow();
    }
} // namespace UKControllerPlugin::MissedApproach
