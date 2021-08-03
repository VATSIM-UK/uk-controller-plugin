#pragma once
#include "pch/pch.h"

namespace UKControllerPlugin {
    namespace Releases {

        typedef struct EnrouteReleaseType
        {
            int id;

            // What to show in the tag
            std::string tagString;

            // The description for menus
            std::string description;

            bool operator==(const EnrouteReleaseType& compare) const
            {
                return this->id == compare.id &&
                    this->tagString == compare.tagString &&
                    this->description == compare.description;
            }
        } EnrouteReleaseType;

    }  // namespace Releases
}  // namespace UKControllerPlugin
