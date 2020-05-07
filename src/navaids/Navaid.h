#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Navaids {

        /*
            Struct containing data about a Navaid
        */
        typedef struct Navaid {

            // The database ID of the navaid
            int id;

            // The identifier for the navaid
            std::string identifier;

            bool operator==(const Navaid& compare) const {
                return this->identifier == compare.identifier;
            }

        } Navaid;
    }  // namespace Navaids
}  // namespace UKControllerPlugin
