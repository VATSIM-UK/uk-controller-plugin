#include "ContainsFreeRouteAirspace.h"

namespace UKControllerPlugin::Srd {
    auto RouteContainsFreeRouteAirspace(const std::string& route) -> bool
    {
        return route.find("<FRA>") != std::string::npos;
    }
} // namespace UKControllerPlugin::Srd
