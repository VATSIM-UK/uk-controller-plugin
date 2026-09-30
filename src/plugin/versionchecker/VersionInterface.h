#pragma once

namespace UKControllerPlugin::VersionChecker {

    class VersionInterface
    {
        public:
        virtual auto GetCurrentVersion() -> std::string = 0;
        virtual auto GetLatestVersion() -> std::string = 0;
        virtual auto GetUpdateMessage() -> std::string = 0;
        virtual auto IsUpdateRequired(const std::string& current, const std::string& latest) -> bool;
    };

} // namespace UKControllerPlugin::VersionChecker
