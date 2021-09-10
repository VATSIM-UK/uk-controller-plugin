#include "Clearance.h"

namespace UKControllerPlugin::Oceanic {

    Clearance::Clearance(std::string callsign) : callsign(callsign)
    {
    }
    Clearance::~Clearance() = default;
    Clearance::Clearance(const Clearance& compare)
    {
        this->AssignProperties(compare);
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
