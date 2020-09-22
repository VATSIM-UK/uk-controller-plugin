#pragma once
#include "stands/Stand.h"
#include "stands/CompareStands.h"

namespace UKControllerPlugin {
    namespace Stands {
        /*
            A new class
        */
        class StandEventHandler
        {
            public:
                StandEventHandler(
                    const std::map<
                        std::string,
                        std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands>
                    > stands
                );

            private:

                // A map of airfield ICAO -> stand struct
                const std::map<
                    std::string,
                    std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands>
                > stands;
        };
    }  // namespace Stands
}  // namespace UKControllerPlugin
