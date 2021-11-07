#include "ResolvedHandoff.h"

namespace UKControllerPlugin::Handoff {
    ResolvedHandoff::ResolvedHandoff(std::string callsign, double frequency)
        : callsign(std::move(callsign)), frequency(frequency){};
} // namespace UKControllerPlugin::Handoff
