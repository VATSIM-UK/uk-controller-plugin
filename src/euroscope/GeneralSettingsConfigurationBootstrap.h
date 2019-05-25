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
        class UserSettingAwareCollection;
    }  // namespace Euroscope
    namespace Dialog {
        class DialogManager;
    }  // namespace Dialog
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Configures the GeneralSettingsDialog
        */
        class GeneralSettingsConfigurationBootstrap
        {
            public:

                static void BootstrapPlugin(
                    UKControllerPlugin::Dialog::DialogManager & dialogManager,
                    UKControllerPlugin::Euroscope::UserSetting & userSettings,
                    UKControllerPlugin::Euroscope::UserSettingAwareCollection & userSettingsHandlers
                );

                static void BootstrapRadarScreen(
                    UKControllerPlugin::Plugin::FunctionCallEventHandler & functionCalls,
                    UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplays,
                    UKControllerPlugin::Command::CommandHandlerCollection & commandHandlers,
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager
                );
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
