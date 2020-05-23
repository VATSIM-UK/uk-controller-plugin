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
 
            // The navaid coordinates
            EuroScopePlugIn::CPosition coordinates;

            // Type strings
            const std::string TYPE_FIX = "FIX";
            const std::string TYPE_VOR = "VOR";
            const std::string TYPE_NDB = "NDB";

            bool operator==(const Navaid& compare) const {
                return this->identifier == compare.identifier;
            }

        } Navaid;
    }  // namespace Navaids
}  // namespace UKControllerPlugin
