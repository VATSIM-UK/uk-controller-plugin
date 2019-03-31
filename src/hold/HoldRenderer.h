#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldProfileManager;
        class HoldManager;
        class HoldDisplayFactory;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Handles the rendering of hold displays on a given ASR
        */
        class HoldRenderer : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface,
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:

                HoldRenderer(
                    const UKControllerPlugin::Hold::HoldProfileManager & profileManager,
                    const UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Hold::HoldDisplayFactory & displayFactory,
                    const int screenObjectId,
                    const int toggleCallbackFunctionId
                );

                // Inherited via AsrEventHandlerInterface
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;

                // Inherited via RadarRenderableInterface
                bool IsVisible(void) const override;
                void LeftClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void LoadProfile(unsigned int profileId);
                void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void Move(RECT position, std::string objectDescription) override;
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                // A profile ID to say that all holds should be unloaded
                const unsigned int unloadAllProfileId = 0;

                // The id of the callback function to use for the configuration menu item
                const int toggleCallbackFunctionId;

                // The screen object id that relates to this renderer
                const int screenObjectId;

                // The description to show in our menu item
                const std::string menuItemDescription = "Show Managed Holds";

                // The key in the ASR for the selected hold profile
                const std::string selectedProfileAsrKey = "selectedHoldProfile";

                // The description for the ASR item
                const std::string selectedProfileAsrDescription = "Selected Hold Profile";

            private:

                int GetHoldIdFromObjectDescription(std::string objectDescription) const;
                std::string GetButtonNameFromObjectDescription(std::string objectDescription) const;

                // The selected profile id
                unsigned int profileId = 0;

                // Should the renderer render the holds?
                bool renderHolds = true;
                
                // Manages the holds
                const UKControllerPlugin::Hold::HoldManager & holdManager;

                // Manages the hold profiles
                const UKControllerPlugin::Hold::HoldProfileManager & profileManager;

                // Creates HoldDisplay objects for our consumption
                const UKControllerPlugin::Hold::HoldDisplayFactory & displayFactory;

                // The holds
                std::list<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>> holds;

                // Access to user settings;
                UKControllerPlugin::Euroscope::UserSetting * userSetting;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
