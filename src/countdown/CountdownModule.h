#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Plugin {
        class FunctionCallEventHandler;
    }  // namespace Plugin
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
    namespace Euroscope {
        class AsrEventHandlerCollection;
    }  // namespace Euroscope
    namespace Windows {
        struct GdiplusBrushes;
        class WinApiInterface;
    }  // namespace Windows
    namespace Countdown {
        class CountdownTimer;
    }  // namespace Countdown
}  // namespace UKControllerPlugin
// END


namespace UKControllerPlugin {
    namespace Countdown {

        /*
        Factory for creating history trail renderers.
        */
        class CountdownModule
        {
            public:
                static void BootstrapPlugin(
                    std::shared_ptr<UKControllerPlugin::Countdown::CountdownTimer> & countdownTimer,
                    UKControllerPlugin::Windows::WinApiInterface & windows
                );
                static void BootstrapRadarScreen(
                    UKControllerPlugin::Plugin::FunctionCallEventHandler & eventHandler,
                    UKControllerPlugin::Countdown::CountdownTimer & countdown,
                    UKControllerPlugin::RadarScreen::RadarRenderableCollection & radarRender,
                    UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & screenControls,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes,
                    UKControllerPlugin::Euroscope::AsrEventHandlerCollection & userSettingHandler
                );
        };
    }  // namespace Countdown
}  // namespace UKControllerPlugin
