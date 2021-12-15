#pragma once

namespace UKControllerPlugin::FlightRules {
    using FlightRule = struct FlightRule
    {
        FlightRule(int id, std::string euroscopeKey, std::string description)
            : id(id), euroscopeKey(std::move(euroscopeKey)), description(std::move(description))
        {
        }

        // The id of the flightrule
        const int id;

        // How EuroScope represents this flight rule
        const std::string euroscopeKey;

        // The name
        const std::string description;
    };
} // namespace UKControllerPlugin::FlightRules
