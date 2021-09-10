#include "EnrouteReleaseType.h"

namespace UKControllerPlugin::Releases {
    auto EnrouteReleaseType::operator==(const EnrouteReleaseType& compare) const -> bool
    {
        return this->id == compare.id && this->tagString == compare.tagString &&
               this->description == compare.description;
    }
} // namespace UKControllerPlugin::Releases
