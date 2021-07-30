#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Releases {

        typedef struct EnrouteRelease
        {
            // The type of release
            int releaseType;

            // The release point
            std::string releasePoint;

            // The time at which this item should be cleared
            std::chrono::system_clock::time_point clearTime;

            bool operator==(const EnrouteRelease& compare) const
            {
                return this->releaseType == compare.releaseType &&
                    this->releasePoint == compare.releasePoint &&
                    this->clearTime == compare.clearTime;
            }
        } EnrouteRelease;

    }  // namespace Releases
}  // namespace UKControllerPlugin
