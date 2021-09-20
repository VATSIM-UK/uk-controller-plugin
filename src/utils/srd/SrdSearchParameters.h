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
    };
} // namespace UKControllerPlugin::Srd
