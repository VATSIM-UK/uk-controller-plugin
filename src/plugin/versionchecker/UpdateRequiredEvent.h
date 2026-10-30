#pragma once

namespace UKControllerPlugin::VersionChecker {

    using UpdateRequiredEvent = struct UpdateRequiredEvent
    {
        std::string message, current, latest;
    };

} // namespace UKControllerPlugin::VersionChecker
