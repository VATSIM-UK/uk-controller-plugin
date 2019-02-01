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

    }  // namespace Hold
}  // namespace UKControllerPlugin
