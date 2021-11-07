#include "ResolvedHandoff.h"

namespace UKControllerPlugin::Handoff {
    ResolvedHandoff::ResolvedHandoff(
        std::string callsign,
        double frequency,
        const std::shared_ptr<Controller::ControllerPositionHierarchy>& hierarchy)
        : callsign(std::move(callsign)), frequency(frequency), hierarchy(hierarchy){};
} // namespace UKControllerPlugin::Handoff
