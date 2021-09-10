#include "CachedHandoff.h"

namespace UKControllerPlugin::Handoff {
    CachedHandoff::CachedHandoff() : frequency(""), callsign(""){};

    CachedHandoff::CachedHandoff(std::string frequency, std::string callsign)
        : frequency(frequency), callsign(callsign){};

    auto CachedHandoff::operator==(const CachedHandoff& compare) const -> bool
    {
        return this->callsign == compare.callsign && this->frequency == compare.frequency;
    }
} // namespace UKControllerPlugin::Handoff
