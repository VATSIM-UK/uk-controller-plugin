#include "pch/pch.h"
#include "update/CheckDevelopmentVersion.h"

const std::string NO_VERSION_SPECIFIED = "#VERSION_STRING#";
const std::string NON_RELEASE_BUILD = "non-release-build";

namespace UKControllerPluginUtils::Update {
    bool IsDevelopmentVersion(std::string version)
    {
        return version == NO_VERSION_SPECIFIED || version == NON_RELEASE_BUILD;
    }

    bool IsDevelopmentVersion(const char* version)
    {
        return IsDevelopmentVersion(std::string(version));
    }
} // namespace UKControllerPluginUtils::Update
