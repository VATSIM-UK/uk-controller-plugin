#pragma once

#include "VersionInterface.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::VersionChecker {

    class SectorFileProviderVersion : public VersionInterface
    {
        public:
        SectorFileProviderVersion(Windows::WinApiInterface& windows);

        auto GetCurrentVersion() -> std::string override;
        auto GetLatestVersion() -> std::string override;
        auto GetUpdateMessage() -> std::string override;

        private:
        Windows::WinApiInterface& windows;
    };

} // namespace UKControllerPlugin::VersionChecker
