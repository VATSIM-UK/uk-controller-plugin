#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeFlightplanListInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Departure {
    class DepartureCoordinationList;

    /*
     * A class for the configuration menu that toggles
     * the departure coordination request list.
     */
    class ToggleDepartureCoordinationList : public RadarScreen::ConfigurableDisplayInterface
    {
        public:
        ToggleDepartureCoordinationList(std::shared_ptr<DepartureCoordinationList> list, int callbackId);
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        Plugin::PopupMenuItem GetConfigurationMenuItem() const override;

        private:
        // The list
        std::shared_ptr<DepartureCoordinationList> list;

        // Callback for the list
        const int callbackId;
    };
} // namespace UKControllerPlugin::Departure
