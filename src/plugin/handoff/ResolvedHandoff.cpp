#include "ResolvedHandoff.h"

namespace UKControllerPlugin::Handoff {
    ResolvedHandoff::ResolvedHandoff(
        std::string callsign,
        const std::shared_ptr<Controller::ControllerPosition> resolvedController,
        const std::shared_ptr<Controller::ControllerPositionHierarchy> sidHierarchy,
        const std::shared_ptr<Controller::ControllerPositionHierarchy> airfieldHierarchy)
        : callsign(std::move(callsign)), resolvedController(resolvedController), sidHierarchy(sidHierarchy),
          airfieldHierarchy(airfieldHierarchy){};
} // namespace UKControllerPlugin::Handoff
