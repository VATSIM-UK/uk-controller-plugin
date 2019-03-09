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
            Delete a profile from the manager and the API by id
        */
        bool HoldProfileManager::DeleteProfile(unsigned int id)
        {
            auto existingProfile = this->profiles.find(id);
            if (existingProfile == this->profiles.cend()) {
                LogWarning("Tried to delete non existent profile " + std::to_string(id));
                return false;
            }

            try {
                this->api.DeleteUserHoldProfile(id);
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
            Get a profile by its id
        */
        UKControllerPlugin::Hold::HoldProfile HoldProfileManager::GetProfile(unsigned int id) const
        {
            auto profile = this->profiles.find(id);
            return profile == this->profiles.cend() ? this->invalidProfile : *profile;
        }

        /*
            Save a new hold profile to the API and this manager.
        */
        int HoldProfileManager::SaveNewProfile(std::string name, std::set<unsigned int> holds)
        {
            unsigned int profileId = 0;
            try {
                profileId = this->api.CreateUserHoldProfile(name, holds);
            } catch (ApiException apiException) {
                // Didnt save to the API, stop
                LogError("Failed to create profile on the API");
                return this->invalidProfileNewId;
            }

            this->AddProfile(
                {
                    profileId,
                    name,
                    holds
                }
            );

            LogInfo("Saved new hold profile " + name);
            return profileId;
        }

        /*
            Update a profile on the API and the manager.
        */
        bool HoldProfileManager::UpdateProfile(unsigned int id, std::string newName, std::set<unsigned int> holds)
        {
            auto existingProfile = this->profiles.find(id);
            if (existingProfile == this->profiles.cend()) {
                LogWarning("Tried to update non existent profile " + std::to_string(id));
                return false;
            }

            try {
                this->api.UpdateUserHoldProfile(id, newName, holds);
            } catch (ApiException apiException) {
                // Didnt save to the API, stop
                LogError("Failed to update profile on the API " + newName);
                return false;
            }

            this->profiles.erase(existingProfile);
            this->AddProfile(
                {
                    id,
                    newName,
                    holds
                }
            );

            LogInfo("Updated hold profile " + newName);
            return true;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
