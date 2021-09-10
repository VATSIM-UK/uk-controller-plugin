#include "EnrouteRelease.h"

namespace UKControllerPlugin::Releases {
    auto EnrouteRelease::operator==(const EnrouteRelease& compare) const -> bool
    {
        return this->releaseType == compare.releaseType && this->releasePoint == compare.releasePoint &&
               this->clearTime == compare.clearTime;
    }
} // namespace UKControllerPlugin::Releases
