#pragma once
#include "pch/stdafx.h"
#include "hold/HoldProfileManager.h"

namespace UKControllerPlugin {
    namespace Hold {

        HoldProfileManager::HoldProfileManager()
        {
        }

        bool HoldProfileManager::AddProfile(UKControllerPlugin::Hold::HoldProfile profile)
        {
            if (!this->profiles.insert(profile).second) {
                LogWarning("Duplicate hold profile " + std::to_string(profile.id));
                return false;
            }

            return true;
        }

        /*
            Returns the total number of profiles in use.
        */
        size_t HoldProfileManager::CountProfiles(void) const
        {
            return this->profiles.size();
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
