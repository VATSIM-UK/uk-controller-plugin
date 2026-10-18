#pragma once

#include "VersionInterface.h"

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::VersionChecker {

    class VersionCheckerModule
    {
        public:
        static void BootstrapPlugin(Bootstrap::PersistenceContainer& container);

        private:
        static void Check(VersionInterface&& iface, const char* label);
        static auto GetVersion(std::function<std::string()> function, const char* label) -> std::string;
    };

} // namespace UKControllerPlugin::VersionChecker
