#pragma once

namespace UKControllerPlugin::Srd {
    /*
        Search parameters when performing an SRD search
    */
    using SrdSearchParameters = struct SrdSearchParameters
    {
        std::string origin;

        std::string destination;

        unsigned int requestedLevel = 0;
        
        std::string callsign = "";

        [[nodiscard]] auto operator==(const SrdSearchParameters& compare) const -> bool
        {
            return this->origin == compare.origin && this->destination == compare.destination &&
                   this->requestedLevel == compare.requestedLevel;
        }
    };
} // namespace UKControllerPlugin::Srd
