#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Releases {

        typedef struct EnrouteReleaseType
        {
            int id;

            // What to show in the tag
            std::string tagString;
            
            // The description for menus
            std::string description;
        } EnrouteReleaseType;

    }  // namespace Releases
}  // namespace UKControllerPlugin
