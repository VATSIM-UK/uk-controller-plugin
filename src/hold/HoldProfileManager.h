#pragma once
#include "hold/HoldProfile.h"
#include "hold/CompareHoldProfile.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    }  // namespace Api
}  // namespace UKControllerPlugin


namespace UKControllerPlugin {
    namespace Hold {

        /*
            Looks after each of the hold profiles.
        */
        class HoldProfileManager
        {
            public:
                explicit HoldProfileManager(const UKControllerPlugin::Api::ApiInterface & api);
                bool AddProfile(UKControllerPlugin::Hold::HoldProfile profile);
                size_t CountProfiles(void) const;
                bool DeleteProfile(unsigned int id);
                UKControllerPlugin::Hold::HoldProfile GetProfile(unsigned int id) const;
                int SaveNewProfile(std::string name, std::set<unsigned int> holds);
                bool UpdateProfile(unsigned int id, std::string newName, std::set<unsigned int> holds);

                // Public type definitions for a custom iterator over the class.
                typedef std::set<
                    UKControllerPlugin::Hold::HoldProfile,
                    UKControllerPlugin::Hold::CompareHoldProfile
                > HoldProfiles;
                typedef HoldProfiles::const_iterator const_iterator;
                const_iterator cbegin(void) const { return profiles.cbegin(); }
                const_iterator cend(void) const { return profiles.cend(); }

                // The invalid profile for returning when a profile does not exist
                const UKControllerPlugin::Hold::HoldProfile invalidProfile = { 0, "INVALID", {} };

                // The ID that gets returned if a new profile save doesnt work
                const int invalidProfileNewId = -1;

            private:

                // The web API for saving profiles
                const UKControllerPlugin::Api::ApiInterface & api;

                // The hold profiles
                std::set<
                    UKControllerPlugin::Hold::HoldProfile,
                    UKControllerPlugin::Hold::CompareHoldProfile
                > profiles;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
