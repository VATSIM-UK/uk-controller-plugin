#include "Clearance.h"

namespace UKControllerPlugin::Oceanic {

    Clearance::Clearance(
        std::string callsign,
        std::string status,
        std::string track,
        std::string entryFix,
        std::string flightLevel,
        std::string mach,
        std::string entryTime,
        std::string clearanceIssued,
        std::string extra)
        : callsign(std::move(callsign)), status(std::move(status)), track(std::move(track)),
          entryFix(std::move(entryFix)), flightLevel(std::move(flightLevel)), mach(std::move(mach)),
          entryTime(std::move(entryTime)), clearanceIssued(std::move(clearanceIssued)), extra(std::move(extra))
    {
    }

    Clearance::Clearance(std::string callsign) : callsign(std::move(callsign))
    {
    }
    Clearance::~Clearance() = default;
    Clearance::Clearance(const Clearance& compare)
    {
        this->AssignProperties(compare);
    }
    Clearance::Clearance(Clearance&& compare) noexcept
    {
        this->MoveProperties(compare);
    }

    auto Clearance::Clearance::operator=(Clearance&& compare) noexcept -> Clearance&
    {
        if (this == &compare) {
            return *this;
        }

        this->MoveProperties(compare);
        return *this;
    }

    void Clearance::Clearance::AssignProperties(const Clearance& compare)
    {
        this->callsign = compare.callsign;
        this->status = compare.status;
        this->track = compare.track;
        this->entryFix = compare.entryFix;
        this->flightLevel = compare.flightLevel;
        this->mach = compare.mach;
        this->entryTime = compare.entryTime;
        this->clearanceIssued = compare.clearanceIssued;
        this->extra = compare.extra;
    }

    void Clearance::Clearance::MoveProperties(Clearance& compare)
    {
        this->callsign = std::move(compare.callsign);
        this->status = std::move(compare.status);
        this->track = std::move(compare.track);
        this->entryFix = std::move(compare.entryFix);
        this->flightLevel = std::move(compare.flightLevel);
        this->mach = std::move(compare.mach);
        this->entryTime = std::move(compare.entryTime);
        this->clearanceIssued = std::move(compare.clearanceIssued);
        this->extra = std::move(compare.extra);
    }

    auto Clearance::operator==(const Clearance& compare) const -> bool
    {
        return this->callsign == compare.callsign;
    }

    auto Clearance::operator=(const Clearance& compare) -> Clearance&
    {
        if (this == &compare) {
            return *this;
        }

        this->AssignProperties(compare);
        return *this;
    }
} // namespace UKControllerPlugin::Oceanic
