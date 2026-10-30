#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Command {
        class CommandHandlerCollection;
    } // namespace Command
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Euroscope {
        class UserSetting;
        class UserSettingAwareCollection;
    } // namespace Euroscope
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Configures the GeneralSettingsDialog
        */
        class GeneralSettingsConfigurationBootstrap
        {
            public:
            static void BootstrapPlugin(Bootstrap::PersistenceContainer& container);

            static void BootstrapRadarScreen(
                UKControllerPlugin::Plugin::FunctionCallEventHandler& functionCalls,
                UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurableDisplays,
                UKControllerPlugin::Command::CommandHandlerCollection& commandHandlers,
                const UKControllerPlugin::Dialog::DialogManager& dialogManager);
        };
    } // namespace Euroscope
} // namespace UKControllerPlugin
