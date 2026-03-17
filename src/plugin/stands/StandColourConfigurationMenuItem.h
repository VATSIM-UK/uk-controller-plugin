#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "plugin/PopupMenuItem.h"

namespace UKControllerPlugin::Stands {
    class StandColourConfiguration;

    /*
        Menu item for configuring stand assignment source colours.
        Allows users to customize the colours used for different stand assignment sources
        (User, Reservation Allocator, VAA, System) without requiring a plugin reload.
    */
    class StandColourConfigurationMenuItem : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
    {
        public:
        StandColourConfigurationMenuItem(
            std::shared_ptr<StandColourConfiguration> colourConfiguration, int callbackFunctionId);

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT area) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        private:
        // The colour configuration to customize
        std::shared_ptr<StandColourConfiguration> colourConfiguration;

        // The item description
        const std::string itemDescription = "Configure Stand Colours";

        // The callback function ID
        const int callbackFunctionId;
    };
} // namespace UKControllerPlugin::Stands
