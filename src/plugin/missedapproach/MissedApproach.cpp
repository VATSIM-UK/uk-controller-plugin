#include "MissedApproach.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::MissedApproach {
    MissedApproach::MissedApproach(int id, std::string callsign, std::chrono::system_clock::time_point expiresAt)
        : id(id), callsign(std::move(callsign)), createdAt(Time::TimeNow()), expiresAt(expiresAt)
    {
    }

    auto MissedApproach::Callsign() const -> const std::string&
    {
        return callsign;
    }

    auto MissedApproach::CreatedAt() const -> const std::chrono::system_clock::time_point&
    {
        return createdAt;
    }

    auto MissedApproach::ExpiresAt() const -> const std::chrono::system_clock::time_point&
    {
        return expiresAt;
    }

    auto MissedApproach::IsExpired() const -> bool
    {
        return expiresAt < Time::TimeNow();
    }

    auto MissedApproach::Id() const -> int
    {
        return this->id;
    }
} // namespace UKControllerPlugin::MissedApproach
