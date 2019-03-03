#pragma once
#include "pch/stdafx.h"
#include "hold/HoldProfileManager.h"
#include "api/ApiInterface.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Hold::HoldProfile;
using UKControllerPlugin::Api::ApiException;

namespace UKControllerPlugin {
    namespace Hold {

        HoldProfileManager::HoldProfileManager(const ApiInterface & api)
            : api(api)
        {
        }

        /*
            Add a profile to the manager only
        */
        bool HoldProfileManager::AddProfile(HoldProfile profile)
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

        /*
            Delete a profile from the manager and the API.
        */
        bool HoldProfileManager::DeleteProfile(std::string name)
        {
            auto existingProfile = this->profiles.find(name);
            if (existingProfile == this->profiles.cend()) {
                LogWarning("Tried to delete non existent profile " + name);
                return false;
            }

            try {
                this->api.DeleteUserHoldProfile(existingProfile->id);
            }
            catch (ApiException apiException) {
             // Didnt save to the API, stop
                LogError("Failed to create profile on the API");
                return false;
            }

            this->profiles.erase(existingProfile);
            return true;
        }

        /*
            Get a profile by its name
        */
        HoldProfile HoldProfileManager::GetProfileByName(std::string name) const
        {
            auto profile = this->profiles.find(name);
            return profile == this->profiles.cend() ? this->invalidProfile : *profile;
        }

        /*
            Save a new hold profile to the API and this manager.
        */
        bool HoldProfileManager::SaveNewProfile(std::string name, std::set<unsigned int> holds)
        {
            if (this->profiles.find(name) != this->profiles.cend()) {
                return false;
            }

            unsigned int profileId = 0;
            try {
                profileId = this->api.CreateUserHoldProfile(name, holds);
            } catch (ApiException apiException) {
                // Didnt save to the API, stop
                LogError("Failed to create profile on the API");
                return false;
            }

            this->AddProfile(
                {
                    profileId,
                    name,
                    holds
                }
            );

            LogInfo("Saved new hold profile " + name);
            return true;
        }

        /*
            Update a profile on the API and the manager.
        */
        bool HoldProfileManager::UpdateProfile(std::string oldName, std::string newName, std::set<unsigned int> holds)
        {
            auto existingProfile = this->profiles.find(oldName);
            if (existingProfile == this->profiles.cend()) {
                LogWarning("Tried to update non existent profile " + oldName);
                return false;
            }

            unsigned int profileId = existingProfile->id;
            try {
                this->api.UpdateUserHoldProfile(profileId, newName, holds);
            } catch (ApiException apiException) {
                // Didnt save to the API, stop
                LogError("Failed to update profile on the API " + newName);
                return false;
            }

            this->profiles.erase(existingProfile);
            this->AddProfile(
                {
                    profileId,
                    newName,
                    holds
                }
            );

            LogInfo("Updated hold profile " + newName);
            return true;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
