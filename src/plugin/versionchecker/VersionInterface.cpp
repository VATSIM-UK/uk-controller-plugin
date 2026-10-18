#include "VersionInterface.h"

namespace UKControllerPlugin::VersionChecker {

    auto VersionInterface::IsUpdateRequired(const std::string& current, const std::string& latest) -> bool
    {
        return current != latest;
    }

} // namespace UKControllerPlugin::VersionChecker
