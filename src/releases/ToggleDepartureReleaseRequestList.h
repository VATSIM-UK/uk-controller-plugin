#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeFlightplanListInterface;
    } // namespace Euroscope

    namespace Releases {

        /*
         * A class for the configuration menu that toggles
         * the departure release request list.
         */
        class ToggleDepartureReleaseRequestList : public RadarScreen::ConfigurableDisplayInterface
        {
            public:
                ToggleDepartureReleaseRequestList(
                    Euroscope::EuroscopeFlightplanListInterface& list,
                    int callbackId
                );
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                Plugin::PopupMenuItem GetConfigurationMenuItem() const override;

            private:
                Euroscope::EuroscopeFlightplanListInterface& list;

                const int callbackId;
        };
    } // namespace Releases
} // namespace UKControllerPlugin
