#pragma once

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    }  // namespace Bootstrap
    namespace Dependency {
        class DependencyCache;
    }  // namespace Dependency
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
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        extern const std::string dependencyFile;
        extern const int timedEventFrequency;

        void BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            const UKControllerPlugin::Dependency::DependencyCache & dependencies,
            UKControllerPlugin::Message::UserMessager & userMessages
        );

        void BootstrapRadarScreen(
            UKControllerPlugin::Plugin::FunctionCallEventHandler & functionCalls,
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplay
        );

    }  // namespace Hold
}  // namespace UKControllerPlugin
