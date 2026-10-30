#pragma once

#include "VersionInterface.h"

namespace UKControllerPlugin {
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::VersionChecker {

    class ControllerPackVersion : public VersionInterface
    {
        public:
        ControllerPackVersion(Curl::CurlInterface& curl, Windows::WinApiInterface& windows);

        auto GetCurrentVersion() -> std::string override;
        auto GetLatestVersion() -> std::string override;
        auto GetUpdateMessage() -> std::string override;
        auto IsUpdateRequired(const std::string& current, const std::string& latest) -> bool override;

        private:
        Curl::CurlInterface& curl;
        Windows::WinApiInterface& windows;
    };

} // namespace UKControllerPlugin::VersionChecker
