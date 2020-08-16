#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Stands {

        /*
            Represents a stand where an aircraft can park
        */
        typedef struct Stand {

            // The id of the stand in the API
            int id;

            // The identifier of the stand
            std::string identifier;
        } Stand;
    }  // namespace Stands
}  // namespace UKControllerPlugin
