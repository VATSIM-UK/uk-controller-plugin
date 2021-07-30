#pragma once

namespace UKControllerPlugin {
    namespace Airfield {

        /*
            A class for parsing the "true" SIDs from their Euroscope representation.
            Removing sector file deprecation marks such as #.
        */
        class NormaliseSid
        {
            public:
                std::string StripSidDeprecation(std::string sid) const;
                const char sidDeprecationPrefix = '#';
        };
    }  // namespace Airfield
}  // namespace UKControllerPlugin
