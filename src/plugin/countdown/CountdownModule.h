#pragma once

namespace UKControllerPlugin {
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    } // namespace RadarScreen
    namespace Euroscope {
        class AsrEventHandlerCollection;
        class UserSetting;
    } // namespace Euroscope
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Countdown {
        class CountdownTimer;
        class TimerConfigurationManager;
    } // namespace Countdown
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Countdown {

    /*
        Factory for creating history trail renderers.
    */
    class CountdownModule
    {
        public:
        static void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container);

        static void BootstrapRadarScreen(
            UKControllerPlugin::Plugin::FunctionCallEventHandler& eventHandler,
            UKControllerPlugin::Countdown::CountdownTimer& countdown,
            const std::shared_ptr<UKControllerPlugin::Countdown::TimerConfigurationManager>& configManager,
            UKControllerPlugin::RadarScreen::RadarRenderableCollection& radarRender,
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& screenControls,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes,
            UKControllerPlugin::Euroscope::AsrEventHandlerCollection& userSettingHandler);

        static void LoadDefaultUserSettings(UKControllerPlugin::Euroscope::UserSetting& userSetting);
    };
} // namespace UKControllerPlugin::Countdown
