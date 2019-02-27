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

                // Public type definitions for a custom iterator over the class.
                typedef std::set<
                    UKControllerPlugin::Hold::HoldProfile,
                    UKControllerPlugin::Hold::CompareHoldProfileName
                > HoldProfiles;
                typedef HoldProfiles::const_iterator const_iterator;
                const_iterator cbegin(void) const { return profiles.cbegin(); }
                const_iterator cend(void) const { return profiles.cend(); }

            private:

                // The hold profiles
                std::set<
                    UKControllerPlugin::Hold::HoldProfile,
                    UKControllerPlugin::Hold::CompareHoldProfileName
                > profiles;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
