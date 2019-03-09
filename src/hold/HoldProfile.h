#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Represents a profile for the configuration of holds.
        */
        typedef struct HoldProfile
        {
            // The id of the profile
            unsigned int id;

            // The name of the profile
            std::string name;

            // The holds that should be loaded in the profile.
            std::set<unsigned int> holds;

            /*
                Equality - because these are going into combo boxes, we want
                to check for equality based on name and id.
            */
            bool operator==(const HoldProfile & compare) const {
                return this->id == compare.id;
            }
        } HoldProfile;

    }  // namespace Hold
}  // namespace UKControllerPlugin