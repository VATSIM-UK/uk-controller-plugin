#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    namespace Message {
        class UserMessager;
    }  // namespace Message
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
    namespace Plugin {
        class FunctionCallEventHandler;
    }  // namespace Plugin
    namespace Dependency {
        class DependencyProviderInterface;
    }  // namespace Dependency
    namespace Api {
        class ApiInterface;
    }  // namespace Api
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        extern const int selectedHoldTagItemId;
        extern const unsigned int popupMenuTagItemId;
        extern const int timedEventFrequency;

        void BootstrapPlugin(
            const UKControllerPlugin::Dependency::DependencyProviderInterface & dependencyProvider,
            UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Message::UserMessager & userMessages
        );

        void BootstrapRadarScreen(
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplay
        );

    }  // namespace Hold
}  // namespace UKControllerPlugin
