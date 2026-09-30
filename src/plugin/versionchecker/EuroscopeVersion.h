#pragma once

#include "VersionInterface.h"

namespace UKControllerPlugin::VersionChecker {

    class EuroscopeVersion : public VersionInterface
    {
        public:
        EuroscopeVersion() = default;

        auto GetCurrentVersion() -> std::string override;
        auto GetLatestVersion() -> std::string override;
        auto GetUpdateMessage() -> std::string override;
    };

} // namespace UKControllerPlugin::VersionChecker
