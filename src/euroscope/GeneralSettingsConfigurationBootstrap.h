#pragma once

namespace UKControllerPlugin {
    namespace Plugin {
        class FunctionCallEventHandler;
    }  // namespace Plugin
    namespace Command {
        class CommandHandlerCollection;
    }  // namespace Command
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
    namespace Euroscope {
        class UserSetting;
    }  // namespace Euroscope
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Configures the GeneralSettingsDialog
        */
        class GeneralSettingsConfigurationBootstrap
        {
            public:
                static void BootstrapRadarScreen(
                    UKControllerPlugin::Plugin::FunctionCallEventHandler & functionCalls,
                    UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplays,
                    UKControllerPlugin::Command::CommandHandlerCollection & commandHandlers,
                    UKControllerPlugin::Euroscope::UserSetting & userSettings,
                    const UKControllerPlugin::Windows::WinApiInterface & winApi
                );
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
