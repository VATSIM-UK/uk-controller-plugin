#pragma once
#include "hold/HoldProfile.h"
#include "hold/CompareHoldProfileName.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Looks after each of the hold profiles.
        */
        class HoldProfileManager
        {
            public:
                explicit HoldProfileManager();
                bool AddProfile(UKControllerPlugin::Hold::HoldProfile profile);
                size_t CountProfiles(void) const;

            private:

                // The hold profiles
                std::set<
                    UKControllerPlugin::Hold::HoldProfile,
                    UKControllerPlugin::Hold::CompareHoldProfileName
                > profiles;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
