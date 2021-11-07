#include "ResolvedHandoff.h"

namespace UKControllerPlugin::Handoff {
    ResolvedHandoff::ResolvedHandoff(std::string frequency, std::string callsign)
        : frequency(std::move(frequency)), callsign(std::move(callsign)){};

    auto ResolvedHandoff::operator==(const ResolvedHandoff& compare) const -> bool
    {
        return this->callsign == compare.callsign && this->frequency == compare.frequency;
    }
} // namespace UKControllerPlugin::Handoff
