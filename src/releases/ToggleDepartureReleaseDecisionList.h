#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeFlightplanListInterface;
    } // namespace Euroscope

    namespace Releases {
        class DepartureReleaseDecisionList;

        /*
         * A class for the configuration menu that toggles
         * the departure release request list.
         */
        class ToggleDepartureReleaseDecisionList : public RadarScreen::ConfigurableDisplayInterface
        {
            public:
                ToggleDepartureReleaseDecisionList(
                    std::shared_ptr<DepartureReleaseDecisionList> list,
                    int callbackId
                );
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                Plugin::PopupMenuItem GetConfigurationMenuItem() const override;

            private:
                // The list
                std::shared_ptr<DepartureReleaseDecisionList> list;

                // Callback for the list
                const int callbackId;
        };
    } // namespace Releases
} // namespace UKControllerPlugin
