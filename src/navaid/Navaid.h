#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Navaid {

        typedef struct Navaid {

            // The database ID of the navaid
            int id;

            // The identifier for the navaid
            std::string identifier;

        } Navaid;
    }  // namespace Navaid
}  // namespace UKControllerPlugin
